/*
 * game_internals.h
 *
 * Created: 12/10/2015 9:02:14 PM
 *  Author: Harrison
 */ 


#ifndef GAME_INTERNALS_H_
#define GAME_INTERNALS_H_

#include "lib/snes/snes.h"

// LED Ports and Pins
#define GAME_LED_RED_PORT		D
#define GAME_LED_RED_PIN		6
#define GAME_LED_GREEN_PORT		D
#define GAME_LED_GREEN_PIN		3
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
#define GAME_BUTTON_YELLOW	SNES_BTN_Y
#define GAME_BUTTON_GREEN   SNES_BTN_X
#define GAME_BUTTON_RED     SNES_BTN_A
#define GAME_BUTTON_BLUE    SNES_BTN_B
#define GAME_BUTTON_LEFT    SNES_BTN_LEFT
#define GAME_BUTTON_RIGHT   SNES_BTN_RIGHT
#define GAME_BUTTON_UP      SNES_BTN_UP
#define GAME_BUTTON_DOWN    SNES_BTN_DOWN
#define GAME_BUTTON_START   SNES_BTN_START
#define GAME_BUTTON_SELECT  SNES_BTN_SELECT
#define GAME_BUTTON_LB      SNES_BTN_L
#define GAME_BUTTON_RB      SNES_BTN_R

// Game States
#define GAME_SCREEN_BEGIN       begin
#define GAME_SCREEN_PLAYERTURN  turn
#define GAME_SCREEN_PLAYBACK    playback
#define GAME_SCREEN_ROUNDOVER   roundover
#define GAME_SCREEN_GAMEOVER    gameover
#define GAME_SCREEN_NAME_SELECT name_select

// Game Configuration Settings
#define GAME_MAX_MOVES 256
#define GAME_SCORE_LIMIT 2
#define GAME_PLAYBACK_DELAY 500
#define GAME_MOVE_TIMEOUT 5000
#define GAME_PAGE_SCROLL_SPEED 4000
//#define GAME_LIGHT_FLICKER_SPEED 1000

// Players 1 and 2, including the ability to select no player
#define GAME_PLAYER_1 0
#define GAME_PLAYER_2 1
#define GAME_PLAYER_1_STR "1"
#define GAME_PLAYER_2_STR "2"
#define GAME_PLAYER_NONE 2

// Internal Prototypes
void game_set_led(uint8_t move);
void game_set_leds(uint8_t leds);
void game_print_lcd_border();
void game_print_scores(char* buffer);
void game_clear_screen();
void game_print_lcd(uint8_t x, uint8_t y, char* string);
void game_print_buffer(char* buffer, uint8_t x, uint8_t y, char* string);
void game_print_buffer_center(char* buffer, uint8_t y, char* string);
char* game_get_p_str(uint8_t player);
uint8_t game_get_turn();
void game_advance_turn();
void game_reset_scores();
uint8_t game_get_move_count();
void game_round_initialize();
uint8_t game_get_winner();
void game_set_winner(uint8_t winner);
void game_set_winner_and_dont_change_any_shit(uint8_t winner);
uint8_t game_get_ns();
void game_set_ns(uint8_t player);
uint8_t game_is_score_limit_reached();
void game_set_move(uint8_t move, uint8_t pos);
uint8_t game_get_move(uint8_t pos);
uint8_t game_button_is_down(uint8_t player, uint8_t button);
void game_tick();
uint8_t game_get_audio_track();



#endif /* GAME_INTERNALS_H_ */