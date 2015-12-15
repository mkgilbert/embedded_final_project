/*
 * roundover_screen.c
 *
 * Created: 12/12/2015 2:41:36 PM
 *  Author: Harrison
 */ 

#include <stdint.h>
#include "../game_internals.h"
#include "../screen.h"
#include "lib/tasks/tasks.h"
#include "roundover_screen.h"
#include "turn_screen.h"
#include "gameover_screen.h"

uint8_t roundover_ticker_task, roundover_page = 0;

void roundover_ticker_tick();

void roundover_init() {
	// Set the display page to 0
	roundover_page = 0;
}

void roundover_begin() {
	// Begin the page ticker
	roundover_ticker_task = task_create(roundover_ticker_tick, ROUNDOVER_SCREEN_TICKER_SPEED, 1);
	game_set_leds(GAME_NO_COLORS);
}

void roundover_render(char* buffer) {
	// Render the current page to the buffer
	switch (roundover_page) {
		case 0:
			game_print_buffer(buffer, 0, 0, "   ROUND OVER   ");
			game_print_buffer(buffer, 0, 1, "        WON!    ");
			game_print_buffer(buffer, 4, 1, game_get_p_str(game_get_winner()));
			break;
		case 1:
			game_print_scores(buffer);
			break;
		case 2:
			game_print_buffer(buffer, 0, 0, "    Press Start ");
			game_print_buffer(buffer, 0, 1, "   To Continue  ");
			uint8_t start_player = GAME_PLAYER_1;
			if (game_get_winner() == GAME_PLAYER_1)
				start_player = GAME_PLAYER_2;
			game_print_buffer(buffer, 0, 0, game_get_p_str(start_player));
			break;
	}
}

void roundover_update() {
	uint8_t start_player = GAME_PLAYER_1;
	if (game_get_winner() == GAME_PLAYER_1)
		start_player = GAME_PLAYER_2;
	if (game_button_is_down(start_player, GAME_BUTTON_START)) {
		if (!game_is_score_limit_reached()) {
			game_round_initialize();
			screen_transition_next(GAME_SCREEN_PLAYERTURN, SCREEN_TRANSITION_NONE, 0);
		} else {
			screen_transition_next(GAME_SCREEN_GAMEOVER, SCREEN_TRANSITION_NONE, 0);
		}
	}
}

void roundover_stop() {
	// Clean up the ticker task
	task_delete(roundover_ticker_task);
}

void roundover_destroy() {
	
}


void roundover_ticker_tick() {
	// Advance the page
	roundover_page = ++roundover_page % ROUNDOVER_SCREEN_NUMBER_PAGES;
}