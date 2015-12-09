/*
 * CFile1.c
 *
 * Created: 12/8/2015 8:35:22 PM
 *  Author: Harrison
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "lib/tasks/tasks.h"
#include "lib/print_tools/colors.h"
#include "lib/print_tools/print_tools.h"
#include "lib/port_helpers/port_helpers.h"

// LED Ports and Pins
#define GAME_LED_RED_PORT		D
#define GAME_LED_RED_PIN		6
#define GAME_LED_GREEN_PORT		D
#define GAME_LED_GREEN_PIN		5
#define GAME_LED_YELLOW_PORT	D
#define GAME_LED_YELLOW_PIN		4
#define GAME_LED_BLUE_PORT		D
#define GAME_LED_BLUE_PIN		7

#define LED_STATUS_PORT A
#define LED_STATUS_PIN 0

// Move Colors
#define GAME_YELLOW	0b0001
#define GAME_GREEN	0b0010
#define GAME_RED	0b0100
#define GAME_BLUE	0b1000
#define GAME_ALL_COLORS (GAME_YELLOW | GAME_GREEN | GAME_RED | GAME_BLUE)
#define GAME_NO_COLORS 0

// Moves
#define GAME_MOVE_YELLOW	0
#define GAME_MOVE_GREEN		1
#define GAME_MOVE_RED		2
#define GAME_MOVE_BLUE		3
#define GAME_MOVE_NONE		4

// Game buttons
#define GAME_BUTTON_YELLOW	'a'
#define GAME_BUTTON_GREEN   'w'
#define GAME_BUTTON_RED     'd'
#define GAME_BUTTON_BLUE    's'
#define GAME_BUTTON_LEFT    'h'
#define GAME_BUTTON_RIGHT   'k'
#define GAME_BUTTON_UP      'u'
#define GAME_BUTTON_DOWN    'j'
#define GAME_BUTTON_START   'z'
#define GAME_BUTTON_SELECT  'x'
#define GAME_BUTTON_LB      '1'
#define GAME_BUTTON_RB      '2'

// Game States
#define GAME_STATE_BEGIN		0
#define GAME_STATE_TURN		    1
#define GAME_STATE_PLAYBACK		3
#define GAME_STATE_GAMEOVER		4
#define GAME_STATE_ROUNDOVER	5

// Game Configuration Settings
#define GAME_MAX_MOVES 256
#define GAME_SCORE_LIMIT 1
#define GAME_PLAYBACK_DELAY 1000
#define GAME_MOVE_TIMEOUT 5000
#define GAME_PAGE_SCROLL_SPEED 4000
#define GAME_LIGHT_FLICKER_SPEED 1000

// Players 1 and 2, including the ability to select no player
#define GAME_PLAYER_1 0
#define GAME_PLAYER_2 1
#define GAME_PLAYER_NONE 2

// Global variables
uint8_t game_p1_score = 0, game_p2_score = 0;
uint8_t game_state = GAME_STATE_BEGIN;
uint8_t game_move_buffer[GAME_MAX_MOVES / 4];
uint8_t game_move_buffer_count = 0;
uint8_t game_turn = 0;
uint8_t game_winner = GAME_PLAYER_NONE;
uint8_t game_number_pages = 1;
uint8_t game_page_scroll_accumulator = 0;
uint8_t game_page_scroll_accumulator_prev = 0;
uint8_t game_light_flicker_status = 0;

// Internal Prototypes
void game_start_screen();
void game_player_turn();
void game_playback();
void game_round_over();
void game_game_over();
void game_set_led(uint8_t move);
void game_set_leds(uint8_t leds);
void game_print_lcd_border();
void game_print_scores();
void game_clear_screen();
void game_print_lcd(uint8_t x, uint8_t y, char* string);
void game_increment_page();
void game_light_flicker();
void game_turn_timeout();
void game_round_initialize();
void game_set_move(uint8_t move, uint8_t pos);
uint8_t game_get_move(uint8_t pos);
uint8_t game_button_is_down(uint8_t player, uint8_t button);

// Game's entry point
void game_run() {
	// Set all leds to output
	PORT_OUTPUT(GAME_LED_RED);
	PORT_OUTPUT(GAME_LED_BLUE);
	PORT_OUTPUT(GAME_LED_GREEN);
	PORT_OUTPUT(GAME_LED_YELLOW);
	
	PORT_OUTPUT(LED_STATUS);
	
	// Main game loop, initiates game screens
	while (1) {
		
		// Clear screen and leds and stuff
		pt_clear_screen();
		pt_set_color_modifier(COLOR_RESET_ALL);
		pt_set_color_modifier(FORE(RED));
		game_print_lcd_border();
		pt_set_color_modifier(FORE(GREEN));
		game_set_leds(0);
		game_clear_screen();
		
		game_page_scroll_accumulator = 0;
		game_page_scroll_accumulator_prev = 255;
		game_number_pages = 1;
		game_light_flicker_status = 0;
		
		// Start the correct game state
		switch (game_state) {
			case GAME_STATE_BEGIN:
			game_start_screen();
			break;
			case GAME_STATE_TURN:
			game_player_turn();
			break;
			case GAME_STATE_PLAYBACK:
			game_playback();
			break;
			case GAME_STATE_ROUNDOVER:
			game_round_over();
			break;
			case GAME_STATE_GAMEOVER:
			game_game_over();
		}
	}
}

// Pre-game start screen
void game_start_screen() {
	game_number_pages = 2;
	uint8_t page_scroll_task = task_create(game_increment_page, GAME_PAGE_SCROLL_SPEED, 1);
	while (1) {
		task_update();
		if (game_page_scroll_accumulator != game_page_scroll_accumulator_prev) {
			game_page_scroll_accumulator_prev = game_page_scroll_accumulator;
			game_clear_screen();
			switch(game_page_scroll_accumulator) {
				case 0:
				game_print_scores();
				break;
				case 1:
				game_print_lcd(0, 0, "Welcome to Simon");
				game_print_lcd(0, 1, " P1 Press Start ");
				break;
			}
		}
		if (game_button_is_down(0, GAME_BUTTON_START)) {
			game_state = GAME_STATE_TURN;
			game_round_initialize();
			break;
		}
	}
	task_delete(page_scroll_task);
}

void game_player_turn() {
	game_print_scores();
	game_print_lcd(1, game_turn, ">");
	uint8_t current_move = 0;
	uint8_t timeout_timer = task_create(game_turn_timeout, GAME_MOVE_TIMEOUT, 0);
	uint8_t input, move;
	while (current_move <= game_move_buffer_count && game_winner == GAME_PLAYER_NONE) {
		task_update();
		input = getchar();
		move = GAME_MOVE_NONE;
		switch (input) {
			case GAME_BUTTON_RED: move = GAME_MOVE_RED; break;
			case GAME_BUTTON_BLUE: move = GAME_MOVE_BLUE; break;
			case GAME_BUTTON_GREEN: move = GAME_MOVE_GREEN; break;
			case GAME_BUTTON_YELLOW: move = GAME_MOVE_YELLOW; break;
		}
		if (move != GAME_MOVE_NONE && current_move < game_move_buffer_count) {
			// Compare move to existing moves
			if (move != game_get_move(current_move++)) {
				if (game_turn == GAME_PLAYER_1) {
					game_winner = GAME_PLAYER_2;
				} else {
					game_winner = GAME_PLAYER_1;
				}
			}
		} else if (move != GAME_MOVE_NONE) {
			// Add color to moves
			game_set_move(move, current_move++);
			task_reset(timeout_timer);
		}
		
		if (move) {
			game_set_led(move);
		}
	}
	
	if (game_winner != GAME_PLAYER_NONE) {
		game_state = GAME_STATE_ROUNDOVER;
		
		if (game_winner == GAME_PLAYER_1) {
			game_p1_score++;
		} else if (game_winner == GAME_PLAYER_2) {
			game_p2_score++;
		}
	} else {
		game_state = GAME_STATE_PLAYBACK;
		if (game_turn == GAME_PLAYER_1) {
			game_turn = GAME_PLAYER_2;
		} else {
			game_turn = GAME_PLAYER_1;
		}
		game_move_buffer_count++;
	}
	task_delete(timeout_timer);
	
}
void game_turn_timeout() {
	if (game_turn == GAME_PLAYER_1) {
		game_winner = GAME_PLAYER_2;
	} else {
		game_winner = GAME_PLAYER_1;
	}
}

void game_playback() {
	
	game_print_lcd(0, 0, "    PLAYER X    ");
	game_print_lcd(0, 1, "   GET READY!   ");
	
	char s[2];
	itoa(game_turn + 1, s, 10);
	game_print_lcd(11, 0, s);
	
	game_number_pages = game_move_buffer_count + 1;
	uint8_t page_scroll_task = task_create(game_increment_page, GAME_PLAYBACK_DELAY, 1);
	while (1) {
		task_update();
		if (game_page_scroll_accumulator != game_page_scroll_accumulator_prev) {
			
			game_page_scroll_accumulator_prev = game_page_scroll_accumulator;
			
			if (game_page_scroll_accumulator == game_move_buffer_count) {
				break;
			}
			
			game_set_led(game_get_move(game_page_scroll_accumulator));
		}
	}
	task_delete(page_scroll_task);
	game_state = GAME_STATE_TURN;
}

void game_round_over() {
	
	game_number_pages = 3;
	uint8_t page_scroll_task = task_create(game_increment_page, GAME_PAGE_SCROLL_SPEED, 1);
	uint8_t light_flicker_task = task_create(game_light_flicker, GAME_LIGHT_FLICKER_SPEED, 1);
	while (1) {
		task_update();
		if (game_page_scroll_accumulator != game_page_scroll_accumulator_prev) {
			game_page_scroll_accumulator_prev = game_page_scroll_accumulator;
			game_clear_screen();
			char s[2];
			switch(game_page_scroll_accumulator) {
				case 0:
				game_print_lcd(0, 0, "   ROUND OVER   ");
				game_print_lcd(0, 1, "  PLAYER X WON! ");
				itoa(game_winner + 1, s, 10);
				game_print_lcd(9, 1, s);
				break;
				case 1:
				game_print_lcd(0, 0, " P1 Press Start ");
				game_print_lcd(0, 1, "   To Continue  ");
				break;
				case 2:
				game_print_scores();
				break;
			}
		}
		if (game_button_is_down(0, GAME_BUTTON_START)) {
			if (game_p1_score < GAME_SCORE_LIMIT && game_p2_score < GAME_SCORE_LIMIT) {
				game_state = GAME_STATE_TURN;
				game_round_initialize();
			} else {
				game_state = GAME_STATE_GAMEOVER;
			}
			break;
		}
	}
	task_delete(page_scroll_task);
	task_delete(light_flicker_task);
}

void game_game_over() {
	
	game_number_pages = 3;
	uint8_t page_scroll_task = task_create(game_increment_page, GAME_PAGE_SCROLL_SPEED, 1);
	uint8_t light_flicker_task = task_create(game_light_flicker, GAME_LIGHT_FLICKER_SPEED, 1);
	while (1) {
		task_update();
		if (game_page_scroll_accumulator != game_page_scroll_accumulator_prev) {
			game_page_scroll_accumulator_prev = game_page_scroll_accumulator;
			game_clear_screen();
			char s[2];
			switch(game_page_scroll_accumulator) {
				case 0:
				game_print_lcd(0, 0, "    GAME OVER   ");
				game_print_lcd(0, 1, "  PLAYER X WON! ");
				itoa(game_winner + 1, s, 10);
				game_print_lcd(9, 1, s);
				break;
				case 1:
				game_print_lcd(0, 0, " P1 Press Start ");
				game_print_lcd(0, 1, "   To Continue  ");
				break;
				case 2:
				game_print_scores();
				break;
			}
		}
		if (game_button_is_down(0, GAME_BUTTON_START)) {
			game_state = GAME_STATE_BEGIN;
			break;
		}
	}
	task_delete(page_scroll_task);
	task_delete(light_flicker_task);
	
}

void game_round_initialize() {
	memset(game_move_buffer, 0, GAME_MAX_MOVES / 4);
	game_move_buffer_count = 0;
	game_turn = GAME_PLAYER_1;
	game_winner = GAME_PLAYER_NONE;
}

void game_set_move(uint8_t move, uint8_t pos) {
	uint8_t slot = pos / 4;
	pos = (pos % 4) * 2;
	
	uint8_t mask = 0b11;
	game_move_buffer[slot] &= ~(mask << pos);
	game_move_buffer[slot] |= (move << pos);
}

uint8_t game_get_move(uint8_t pos) {
	uint8_t slot = pos / 4;
	pos = (pos % 4) * 2;
	
	uint8_t mask = 0b11;
	return (game_move_buffer[slot] & (mask << pos)) >> pos;
}

// Increments the page scroll accumulator
void game_increment_page() {
	game_page_scroll_accumulator++;
	if (game_page_scroll_accumulator >= game_number_pages)
		game_page_scroll_accumulator = 0;
}

void game_light_flicker() {
	if (game_light_flicker_status) {
		game_light_flicker_status = 0;
		game_set_leds(GAME_NO_COLORS);
	} else {
		game_light_flicker_status = 1;
		game_set_leds(GAME_ALL_COLORS);
	}
}

uint8_t game_button_is_down(uint8_t player, uint8_t button) {
	return getchar() == button;
}

// Sets which LEDs should be enabled
void game_set_leds(uint8_t leds) {
	
	if (leds & GAME_YELLOW) {
		PORT_SET(GAME_LED_YELLOW);
	} else {
		PORT_UNSET(GAME_LED_YELLOW);
	}
	
	if (leds & GAME_GREEN) {
		PORT_SET(GAME_LED_GREEN);
	} else {
		PORT_UNSET(GAME_LED_GREEN);
	}
	
	if (leds & GAME_RED) {
		PORT_SET(GAME_LED_RED);
	} else {
		PORT_UNSET(GAME_LED_RED);
	}
	
	if (leds & GAME_BLUE) {
		PORT_SET(GAME_LED_BLUE);
	} else {
		PORT_UNSET(GAME_LED_BLUE);
	}
}

void game_set_led(uint8_t move) {
	if (move == GAME_MOVE_YELLOW) {
		game_set_leds(GAME_YELLOW);
	}
	
	if (move == GAME_MOVE_GREEN) {
		game_set_leds(GAME_GREEN);
	}
	
	if (move == GAME_MOVE_RED) {
		game_set_leds(GAME_RED);
	}
	
	if (move == GAME_MOVE_BLUE) {
		game_set_leds(GAME_BLUE);
	}
}

// Draws the border around the LCD screen - uart representation only
void game_print_lcd_border() {
	int i;
	pt_set_cursor_pos(2, 0);
	for (i = 0; i < 16; i++) {
		printf("-");
	}
	pt_set_cursor_pos(2, 3);
	for (i = 0; i < 16; i++) {
		printf("-");
	}
	
	for (i = 0; i < 4; i++) {
		pt_set_cursor_pos(0, i);
		printf(" |");
		pt_set_cursor_pos(18, i);
		printf("|");
	}
}

// Prints the scoreboard on the screen
void game_print_scores() {
	char num[4];
	
	game_clear_screen();
	
	itoa(game_p1_score, num, 10);
	game_print_lcd(0, 0, "  Player 1: ");
	game_print_lcd(12, 0, num);
	
	itoa(game_p2_score, num, 10);
	game_print_lcd(0, 1, "  Player 2: ");
	game_print_lcd(12, 1, num);
}

// Clears the LCD screen
void game_clear_screen() {
	
	game_print_lcd(0, 0, "                ");
	game_print_lcd(0, 1, "                ");
	
}

// Print a string to the screen
void game_print_lcd(uint8_t x, uint8_t y, char* string) {
	
	pt_set_cursor_pos(x + 2, y + 1);
	printf("%s", string);
	
}