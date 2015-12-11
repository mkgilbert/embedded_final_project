/*
 * game_internals.h
 *
 * Created: 12/10/2015 9:02:14 PM
 *  Author: Harrison
 */ 


#ifndef GAME_INTERNALS_H_
#define GAME_INTERNALS_H_

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
// #define GAME_STATE_BEGIN		0
// #define GAME_STATE_TURN		    1
// #define GAME_STATE_PLAYBACK		3
// #define GAME_STATE_GAMEOVER		4
// #define GAME_STATE_ROUNDOVER	5
#define GAME_SCREEN_BEGIN      begin
#define GAME_SCREEN_PLAYERTURN turn

// Game Configuration Settings
#define GAME_MAX_MOVES 256
#define GAME_SCORE_LIMIT 1
#define GAME_PLAYBACK_DELAY 1000
#define GAME_MOVE_TIMEOUT 5000
#define GAME_PAGE_SCROLL_SPEED 4000
//#define GAME_LIGHT_FLICKER_SPEED 1000

// Players 1 and 2, including the ability to select no player
#define GAME_PLAYER_1 0
#define GAME_PLAYER_2 1
#define GAME_PLAYER_NONE 2

// Internal Prototypes
//void game_start_screen();
//void game_player_turn();
//void game_playback();
//void game_round_over();
//void game_game_over();
void game_set_led(uint8_t move);
void game_set_leds(uint8_t leds);
void game_print_lcd_border();
void game_print_scores();
void game_clear_screen();
void game_print_lcd(uint8_t x, uint8_t y, char* string);
//void game_increment_page();
//void game_light_flicker();
//void game_turn_timeout();
//void game_round_initialize();
void game_set_move(uint8_t move, uint8_t pos);
uint8_t game_get_move(uint8_t pos);
uint8_t game_button_is_down(uint8_t player, uint8_t button);
void game_tick();



#endif /* GAME_INTERNALS_H_ */