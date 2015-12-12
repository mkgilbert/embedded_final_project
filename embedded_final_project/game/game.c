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
#include "lib/audio/audio.h"
#include "lib/kb/kb.h"
#include "lib/snes/snes.h"

// Global variables
uint8_t game_p1_score = 0, game_p2_score = 0;
uint8_t game_move_buffer[GAME_MAX_MOVES / 4];
uint8_t game_move_buffer_count = 0;
uint8_t game_turn = 0;
uint8_t game_winner = GAME_PLAYER_NONE;

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
		audio_update();
	}
}

void game_tick() {
	snes_update();
	screen_tick();
}

void game_round_initialize() {
	memset(game_move_buffer, 0, GAME_MAX_MOVES / 4);
	game_move_buffer_count = 0;
	game_turn = GAME_PLAYER_1;
	game_winner = GAME_PLAYER_NONE;
}

void game_reset_scores() {
	game_p1_score = 0;
	game_p2_score = 0;
}

uint8_t game_get_turn() {
	return game_turn;
}

void game_advance_turn() {
	if (game_turn == GAME_PLAYER_1) {
		game_turn = GAME_PLAYER_2;
	} else {
		game_turn = GAME_PLAYER_1;
	}
	game_move_buffer_count++;
}

uint8_t game_get_winner() {
	return game_winner;
}
void game_set_winner(uint8_t winner) {
	game_winner = winner;
	switch (winner) {
		case GAME_PLAYER_1:
			game_p1_score++;
			break;
		case GAME_PLAYER_2:
			game_p2_score++;
			break;
	}
}

uint8_t game_is_score_limit_reached() {
	return game_p1_score >= GAME_SCORE_LIMIT || game_p2_score >= GAME_SCORE_LIMIT;
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

uint8_t game_get_move_count() {
	return game_move_buffer_count;
}

uint8_t game_button_is_down(uint8_t player, uint8_t button) {
	/*
	unsigned char c;
	if (kb_haschar()) {
		c = kb_getchar();
		pt_set_cursor_pos(2, 8);
		printf("%c", c);
		return ((uint8_t)c == button ? 1 : 0);
	}
	return 0;*/
	
	if (player == GAME_PLAYER_1)
		return snes_is_pressed(SNES_CONTROLLER1, button);
	else if (player == GAME_PLAYER_2)
		return snes_is_pressed(SNES_CONTROLLER2, button);
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
	lcd_print(0, x, y, str_to_print);
	
}