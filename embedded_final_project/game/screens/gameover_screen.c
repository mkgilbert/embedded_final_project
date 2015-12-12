/*
 * gameover_screen.c
 *
 * Created: 12/12/2015 2:42:10 PM
 *  Author: Harrison
 */ 

#include <stdint.h>
#include "../game_internals.h"
#include "../screen.h"
#include "lib/tasks/tasks.h"
#include "gameover_screen.h"
#include "begin_screen.h"

uint8_t gameover_ticker_task, gameover_page = 0;

void gameover_ticker_tick();

void gameover_init() {
	// Set the display page to 0
	gameover_page = 0;
}

void gameover_begin() {
	// Begin the page ticker
	gameover_ticker_task = task_create(gameover_ticker_tick, GAMEOVER_SCREEN_TICKER_SPEED, 1);
	game_set_leds(GAME_NO_COLORS);
}

void gameover_render(char* buffer) {
	// Render the current page to the buffer
	switch (gameover_page) {
		case 0:
			game_print_buffer(buffer, 0, 0, "   GAME OVER    ");
			game_print_buffer(buffer, 0, 1, "  PLAYER X WON! ");
			game_print_buffer(buffer, 9, 1, game_get_winner() == GAME_PLAYER_1 ? GAME_PLAYER_1_STR : GAME_PLAYER_2_STR);
			break;
		case 1:
			game_print_scores(buffer);
			break;
		case 2:
			game_print_buffer(buffer, 0, 0, " PX Press Start ");
			game_print_buffer(buffer, 0, 1, "   To Continue  ");
			game_print_buffer(buffer, 2, 0, GAME_PLAYER_1_STR);
			break;
	}
}

void gameover_update() {
	if (game_button_is_down(0, GAME_BUTTON_START)) {
		screen_transition_next(GAME_SCREEN_BEGIN, SCREEN_TRANSITION_NONE, 0);
	}
}

void gameover_stop() {
	// Clean up the ticker task
	task_delete(gameover_ticker_task);
}

void gameover_destroy() {
	
}


void gameover_ticker_tick() {
	// Advance the page
	gameover_page = ++gameover_page % GAMEOVER_SCREEN_NUMBER_PAGES;
}