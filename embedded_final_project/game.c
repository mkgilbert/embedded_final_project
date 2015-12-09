/*
 * CFile1.c
 *
 * Created: 12/8/2015 8:35:22 PM
 *  Author: Harrison
 */ 

#include <avr/io.h>
#include "game.h"
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

// Move Colors
#define GAME_YELLOW	0b0001
#define GAME_GREEN	0b0010
#define GAME_RED	0b0100
#define GAME_BLUE	0b1000
#define GAME_ALL_COLORS (GAME_YELLOW | GAME_GREEN | GAME_RED | GAME_BLUE)

// Game States
#define GAME_STATE_BEGIN		0
#define GAME_STATE_P1_TURN		1
#define GAME_STATE_P2_TURN		2
#define GAME_STATE_PLAYBACK		3
#define GAME_STATE_GAMEOVER		4
#define GAME_STATE_ROUNDOVER	5

// Game Configuration Settings
#define GAME_MAX_MOVES 256
#define GAME_SCORE_LIMIT 3
#define GAME_PLAYBACK_DELAY 100
#define GAME_MOVE_TIMEOUT 5000

// Players 1 and 2, including the ability to select no player
#define GAME_PLAYER_1 0
#define GAME_PLAYER_2 1
#define GAME_PLAYER_NONE 2

// Global variables
uint8_t p1_score = 0, p2_score = 0;
uint8_t game_state = GAME_STATE_BEGIN;
uint8_t move_buffer[GAME_MAX_MOVES / 4];

// Internal Prototypes
void game_start_screen();
void game_set_leds(uint8_t leds);
void game_print_lcd_border();
void game_print_scores(uint8_t p1_score, uint8_t p2_score);
void game_clear_screen();
void game_print_lcd(uint8_t x, uint8_t y, char* string);

// Game's entry point
void game_run() {
	// Set all leds to output
	PORT_OUTPUT(GAME_LED_RED);
	PORT_OUTPUT(GAME_LED_BLUE);
	PORT_OUTPUT(GAME_LED_GREEN);
	PORT_OUTPUT(GAME_LED_YELLOW);
	
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
		
		// Start the correct game state
		switch (game_state) {
			case GAME_STATE_BEGIN:
			game_start_screen();
			break;
		}
	}
}

// Pre-game start screen
void game_start_screen() {
	game_print_scores(p1_score, p2_score);
	while (1) {}
}

// Sets which LEDs should be enabled
void game_set_leds(uint8_t leds) {
	
	if (leds & GAME_YELLOW) {
		PORT_SET(GAME_LED_YELLOW);
	}
	
	if (leds & GAME_GREEN) {
		PORT_SET(GAME_LED_GREEN);
	}
	
	if (leds & GAME_RED) {
		PORT_SET(GAME_LED_RED);
	}
	
	if (leds & GAME_BLUE) {
		PORT_SET(GAME_LED_BLUE);
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
void game_print_scores(uint8_t p1_score, uint8_t p2_score) {
	char num[3];
	
	game_clear_screen();
	
	itoa(p1_score, num, 10);
	game_print_lcd(0, 0, "  Player 1: ");
	game_print_lcd(12, 0, num);
	
	itoa(p2_score, num, 10);
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