/*
 * CFile1.c
 *
 * Created: 12/8/2015 8:35:22 PM
 *  Author: Harrison
 */ 

#include "system.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_internals.h"
#include "screen.h"
#include "screens/begin_screen.h"
#include "screens/turn_screen.h"
#include "lib/tasks/tasks.h"
#include "lib/print_tools/colors.h"
#include "lib/print_tools/print_tools.h"
#include "lib/port_helpers/port_helpers.h"
#include "lib/kb/kb.h"

// Global variables
uint8_t game_p1_score = 0, game_p2_score = 0;
//uint8_t game_state = GAME_STATE_BEGIN;
uint8_t game_move_buffer[GAME_MAX_MOVES / 4];
uint8_t game_move_buffer_count = 0;
uint8_t game_turn = 0;
uint8_t game_winner = GAME_PLAYER_NONE;
//uint8_t game_number_pages = 1;
//uint8_t game_page_scroll_accumulator = 0;
//uint8_t game_page_scroll_accumulator_prev = 0;
//uint8_t game_light_flicker_status = 0;

// Game's entry point
void game_run() {
	
	// Prepare output ports
	PORT_OUTPUT(GAME_LED_RED);
	PORT_OUTPUT(GAME_LED_BLUE);
	PORT_OUTPUT(GAME_LED_GREEN);
	PORT_OUTPUT(GAME_LED_YELLOW);
	
	PORT_OUTPUT(LED_STATUS);
	
	// reset screen and leds
	pt_clear_screen();
	pt_set_color_modifier(COLOR_RESET_ALL);
	pt_set_color_modifier(FORE(RED));
	game_print_lcd_border();
	pt_set_color_modifier(FORE(GREEN));
	game_set_leds(0);
	game_clear_screen();
	
	// Set the first screen to display
	screen_set_current(GAME_SCREEN_BEGIN);
	
	// Set the tick-rate (render / update rate) to 17ms
	uint8_t tick_task = task_create(game_tick, 17, 1);
	
	// Main program loop
	while (1) {
		task_update();
		// audio update
	}
}

void game_tick() {
	// snes update
	screen_tick();
}

//void game_run2() {
	//// Set all leds to output
	//PORT_OUTPUT(GAME_LED_RED);
	//PORT_OUTPUT(GAME_LED_BLUE);
	//PORT_OUTPUT(GAME_LED_GREEN);
	//PORT_OUTPUT(GAME_LED_YELLOW);
	//
	//PORT_OUTPUT(LED_STATUS);
	//
	//// Main game loop, initiates game screens
	//while (1) {
		//
		//// Clear screen and leds and stuff
		//pt_clear_screen();
		//pt_set_color_modifier(COLOR_RESET_ALL);
		//pt_set_color_modifier(FORE(RED));
		//game_print_lcd_border();
		//pt_set_color_modifier(FORE(GREEN));
		//game_set_leds(0);
		//game_clear_screen();
		//
		//game_page_scroll_accumulator = 0;
		//game_page_scroll_accumulator_prev = 255;
		//game_number_pages = 1;
		//game_light_flicker_status = 0;
		//
		//// Start the correct game state
		//switch (game_state) {
			//case GAME_STATE_BEGIN:
			//game_start_screen();
			//break;
			//case GAME_STATE_TURN:
			//game_player_turn();
			//break;
			//case GAME_STATE_PLAYBACK:
			//game_playback();
			//break;
			//case GAME_STATE_ROUNDOVER:
			//game_round_over();
			//break;
			//case GAME_STATE_GAMEOVER:
			//game_game_over();
		//}
	//}
//}
//
//// Pre-game start screen
//void game_start_screen() {
	//game_number_pages = 2;
	//uint8_t page_scroll_task = task_create(game_increment_page, GAME_PAGE_SCROLL_SPEED, 1);
	//while (1) {
		//task_update();
		//if (game_page_scroll_accumulator != game_page_scroll_accumulator_prev) {
			//game_page_scroll_accumulator_prev = game_page_scroll_accumulator;
			//game_clear_screen();
			//switch(game_page_scroll_accumulator) {
				//case 0:
				//game_print_scores();
				//break;
				//case 1:
				//game_print_lcd(0, 0, "Welcome to Simon");
				//game_print_lcd(0, 1, " P1 Press Start ");
				//break;
			//}
		//}
		//if (game_button_is_down(0, GAME_BUTTON_START)) {
			//game_state = GAME_STATE_TURN;
			//game_p1_score = 0;
			//game_p2_score = 0;
			//game_round_initialize();
			//break;
		//}
	//}
	//task_delete(page_scroll_task);
//}
//
//void game_player_turn() {
	//game_print_scores();
	//game_print_lcd(1, game_turn, ">");
	//uint8_t current_move = 0;
	//uint8_t timeout_timer = task_create(game_turn_timeout, GAME_MOVE_TIMEOUT, 0);
	//uint8_t input, move;
	//while (current_move <= game_move_buffer_count && game_winner == GAME_PLAYER_NONE) {
		//task_update();
		//move = GAME_MOVE_NONE;
		//
		//// TODO: Use game_button_is_down
		//if (kb_haschar()) {
			//char input = kb_getchar();
			//switch (input) {
				//case GAME_BUTTON_RED: move = GAME_MOVE_RED; break;
				//case GAME_BUTTON_BLUE: move = GAME_MOVE_BLUE; break;
				//case GAME_BUTTON_GREEN: move = GAME_MOVE_GREEN; break;
				//case GAME_BUTTON_YELLOW: move = GAME_MOVE_YELLOW; break;
			//}
		//}
		//
		////if (game_button_is_down(game_turn, GAME_BUTTON_RED)) {move = GAME_MOVE_RED;}
		////else if (game_button_is_down(game_turn, GAME_BUTTON_BLUE)) {move = GAME_MOVE_BLUE;}
		////else if (game_button_is_down(game_turn, GAME_BUTTON_GREEN)) {move = GAME_MOVE_GREEN;}
		////else if (game_button_is_down(game_turn, GAME_BUTTON_YELLOW)) {move = GAME_MOVE_YELLOW;}
		////
		//if (move != GAME_MOVE_NONE && current_move < game_move_buffer_count) {
			//// Compare move to existing moves
			//if (move != game_get_move(current_move++)) {
				//if (game_turn == GAME_PLAYER_1) {
					//game_winner = GAME_PLAYER_2;
				//} else {
					//game_winner = GAME_PLAYER_1;
				//}
			//}
		//} else if (move != GAME_MOVE_NONE) {
			//// Add color to moves
			//game_set_move(move, current_move++);
			//task_reset(timeout_timer);
		//}
		//
		//if (move != GAME_MOVE_NONE) {
			//game_set_led(move);
		//}
	//}
	//
	//if (game_winner != GAME_PLAYER_NONE) {
		//game_state = GAME_STATE_ROUNDOVER;
		//
		//if (game_winner == GAME_PLAYER_1) {
			//game_p1_score++;
		//} else if (game_winner == GAME_PLAYER_2) {
			//game_p2_score++;
		//}
	//} else {
		//game_state = GAME_STATE_PLAYBACK;
		//if (game_turn == GAME_PLAYER_1) {
			//game_turn = GAME_PLAYER_2;
		//} else {
			//game_turn = GAME_PLAYER_1;
		//}
		//game_move_buffer_count++;
	//}
	//task_delete(timeout_timer);
	//
//}
//void game_turn_timeout() {
	//if (game_turn == GAME_PLAYER_1) {
		//game_winner = GAME_PLAYER_2;
	//} else {
		//game_winner = GAME_PLAYER_1;
	//}
//}
//
//void game_playback() {
	//
	//game_print_lcd(0, 0, "    PLAYER X    ");
	//game_print_lcd(0, 1, "   GET READY!   ");
	//
	//char s[2];
	//itoa(game_turn + 1, s, 10);
	//game_print_lcd(11, 0, s);
	//
	//game_number_pages = game_move_buffer_count + 1;
	//uint8_t page_scroll_task = task_create(game_increment_page, GAME_PLAYBACK_DELAY, 1);
	//while (1) {
		//task_update();
		//if (game_page_scroll_accumulator != game_page_scroll_accumulator_prev) {
			//
			//game_page_scroll_accumulator_prev = game_page_scroll_accumulator;
			//
			//if (game_page_scroll_accumulator == game_move_buffer_count) {
				//break;
			//}
			//
			//game_set_led(game_get_move(game_page_scroll_accumulator));
		//}
	//}
	//task_delete(page_scroll_task);
	//game_state = GAME_STATE_TURN;
//}
//
//void game_round_over() {
	//
	//game_number_pages = 3;
	//uint8_t page_scroll_task = task_create(game_increment_page, GAME_PAGE_SCROLL_SPEED, 1);
	//uint8_t light_flicker_task = task_create(game_light_flicker, GAME_LIGHT_FLICKER_SPEED, 1);
	//while (1) {
		//task_update();
		//if (game_page_scroll_accumulator != game_page_scroll_accumulator_prev) {
			//game_page_scroll_accumulator_prev = game_page_scroll_accumulator;
			//game_clear_screen();
			//char s[2];
			//switch(game_page_scroll_accumulator) {
				//case 0:
				//game_print_lcd(0, 0, "   ROUND OVER   ");
				//game_print_lcd(0, 1, "  PLAYER X WON! ");
				//itoa(game_winner + 1, s, 10);
				//game_print_lcd(9, 1, s);
				//break;
				//case 1:
				//game_print_lcd(0, 0, " P1 Press Start ");
				//game_print_lcd(0, 1, "   To Continue  ");
				//break;
				//case 2:
				//game_print_scores();
				//break;
			//}
		//}
		//if (game_button_is_down(0, GAME_BUTTON_START)) {
			//if (game_p1_score < GAME_SCORE_LIMIT && game_p2_score < GAME_SCORE_LIMIT) {
				//game_state = GAME_STATE_TURN;
				//game_round_initialize();
			//} else {
				//game_state = GAME_STATE_GAMEOVER;
			//}
			//break;
		//}
	//}
	//task_delete(page_scroll_task);
	//task_delete(light_flicker_task);
//}
//
//void game_game_over() {
	//
	//game_number_pages = 3;
	//uint8_t page_scroll_task = task_create(game_increment_page, GAME_PAGE_SCROLL_SPEED, 1);
	//uint8_t light_flicker_task = task_create(game_light_flicker, GAME_LIGHT_FLICKER_SPEED, 1);
	//while (1) {
		//task_update();
		//if (game_page_scroll_accumulator != game_page_scroll_accumulator_prev) {
			//game_page_scroll_accumulator_prev = game_page_scroll_accumulator;
			//game_clear_screen();
			//char s[2];
			//switch(game_page_scroll_accumulator) {
				//case 0:
				//game_print_lcd(0, 0, "    GAME OVER   ");
				//game_print_lcd(0, 1, "  PLAYER X WON! ");
				//itoa(game_winner + 1, s, 10);
				//game_print_lcd(9, 1, s);
				//break;
				//case 1:
				//game_print_lcd(0, 0, " P1 Press Start ");
				//game_print_lcd(0, 1, "   To Continue  ");
				//break;
				//case 2:
				//game_print_scores();
				//break;
			//}
		//}
		//if (game_button_is_down(0, GAME_BUTTON_START)) {
			//game_state = GAME_STATE_BEGIN;
			//break;
		//}
	//}
	//task_delete(page_scroll_task);
	//task_delete(light_flicker_task);
	//
//}
//
//void game_round_initialize() {
	//memset(game_move_buffer, 0, GAME_MAX_MOVES / 4);
	//game_move_buffer_count = 0;
	//game_turn = GAME_PLAYER_1;
	//game_winner = GAME_PLAYER_NONE;
//}

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

//// Increments the page scroll accumulator
//void game_increment_page() {
	//game_page_scroll_accumulator++;
	//if (game_page_scroll_accumulator >= game_number_pages)
		//game_page_scroll_accumulator = 0;
//}
//
//void game_light_flicker() {
	//if (game_light_flicker_status) {
		//game_light_flicker_status = 0;
		//game_set_leds(GAME_NO_COLORS);
	//} else {
		//game_light_flicker_status = 1;
		//game_set_leds(GAME_ALL_COLORS);
	//}
//}

uint8_t game_button_is_down(uint8_t player, uint8_t button) {
	unsigned char c;
	if (kb_haschar()) {
		c = kb_getchar();
		pt_set_cursor_pos(2, 8);
		printf("%c", c);
		return ((uint8_t)c == button ? 1 : 0);
	}
	return 0;
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
void game_print_scores(char* buffer) {
	char num[4];
	
	memset(buffer, ' ', 32);
	
	itoa(game_p1_score, num, 10);
	game_print_buffer(buffer, 0, 0, "  Player 1: ");
	game_print_buffer(buffer, 12, 0, num);
	
	itoa(game_p2_score, num, 10);
	game_print_buffer(buffer, 0, 1, "  Player 2: ");
	game_print_buffer(buffer, 12, 1, num);
	
}

// Clears the LCD screen
void game_clear_screen() {
	
	game_print_lcd(0, 0, "                ");
	game_print_lcd(0, 1, "                ");
	
}

// Prints a string to the buffer provided
void game_print_buffer(char* buffer, uint8_t x, uint8_t y, char* string) {
	memcpy(buffer + ((y * 16) + x), string, strlen(string));
}

// Print a string to the screen
void game_print_lcd(uint8_t x, uint8_t y, char* string) {
	
	// Set the draw position on the screen
	pt_set_cursor_pos(x + 2, y + 1);
	
	// Create a new base string that is 17 characters long and fill with null terminators
	char str_to_print[17];
	memset(str_to_print, 0, 17);
	
	// copy the first 16 characters from the input string and put them into the printed string
	memcpy(str_to_print, string, 16);
	printf("%s", str_to_print);
	
}