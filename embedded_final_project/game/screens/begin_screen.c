/*
 * begin_screen.c
 *
 * Created: 12/10/2015 9:09:35 PM
 *  Author: Harrison
 */ 

#include <stdint.h>
#include <stdio.h>
#include "begin_screen.h"
#include "turn_screen.h"
#include "name_select_screen.h"
#include "../screen.h"
#include "../game_internals.h"
#include "lib/audio/audio.h"
#include "lib/tasks/tasks.h"

uint8_t begin_ticker_task, begin_page = 0;

void begin_ticker_tick();

void begin_init() {
	// Set the display page to 0
	begin_page = 0;
}

void begin_begin() {
	// Begin the page ticker
	begin_ticker_task = task_create(begin_ticker_tick, BEGIN_SCREEN_TICKER_SPEED, 1);
	uint8_t r = rand() % 2;
	if (r == 0)
		game_set_winner_and_dont_change_any_shit(GAME_PLAYER_1);
	else 
		game_set_winner_and_dont_change_any_shit(GAME_PLAYER_2);
}

void begin_render(char* buffer) {
	// Render the current page to the buffer
	switch (begin_page) {
		case 1:
			game_print_scores(buffer);
			break;
		case 0:
			game_print_buffer(buffer, 0, 0, "Welcome to Simon");
			game_print_buffer(buffer, 0, 1, "    Press Start ");
			game_print_buffer(buffer, 0, 1, game_get_p_str(game_get_winner() == GAME_PLAYER_1 ? GAME_PLAYER_2 : GAME_PLAYER_1));
			break;
	}
}

void begin_update() {
	// Check if the start button has been pressed
	// If so, start the game and transition to the next screen.
	if (game_button_is_down(game_get_winner() == GAME_PLAYER_1 ? GAME_PLAYER_2 : GAME_PLAYER_1, GAME_BUTTON_START)) {
		screen_transition_next(GAME_SCREEN_PLAYERTURN, SCREEN_TRANSITION_NONE, 0);
		game_reset_scores();
 		game_round_initialize();
	} else if (game_button_is_down(GAME_PLAYER_1, GAME_BUTTON_SELECT)) {
		game_set_ns(GAME_PLAYER_1);
		screen_transition_next(GAME_SCREEN_NAME_SELECT, SCREEN_TRANSITION_NONE, 0);
	} else if (game_button_is_down(GAME_PLAYER_2, GAME_BUTTON_SELECT)) {
		game_set_ns(GAME_PLAYER_2);
		screen_transition_next(GAME_SCREEN_NAME_SELECT, SCREEN_TRANSITION_NONE, 0);
	} else if (game_button_is_down(GAME_PLAYER_1, GAME_BUTTON_LB) || game_button_is_down(GAME_PLAYER_2, GAME_BUTTON_LB)){
		audio_stop(AUDIO_TRACK_0);
	} else if (game_button_is_down(GAME_PLAYER_1, GAME_BUTTON_RB) || game_button_is_down(GAME_PLAYER_2, GAME_BUTTON_RB)){
		audio_stop(AUDIO_TRACK_0);
	}
}

void begin_stop() {
	// Clean up the ticker task
	task_delete(begin_ticker_task);
}

void begin_destroy() {
	
}


void begin_ticker_tick() {
	// Advance the page
	begin_page = ++begin_page % BEGIN_SCREEN_NUMBER_PAGES;
}