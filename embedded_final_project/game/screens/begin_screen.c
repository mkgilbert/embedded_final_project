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
#include "../screen.h"
#include "../game_internals.h"
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
}

void begin_render(char* buffer) {
	// Render the current page to the buffer
	switch (begin_page) {
		case 0:
			game_print_scores(buffer);
			break;
		case 1:
			game_print_buffer(buffer, 0, 0, "Welcome to Simon");
			game_print_buffer(buffer, 0, 1, " PX Press Start ");
			game_print_buffer(buffer, 2, 1, GAME_PLAYER_1_STR);
			break;
	}
}

void begin_update() {
	// Check if the start button has been pressed
	// If so, start the game and transition to the next screen.
	if (game_button_is_down(GAME_PLAYER_1, GAME_BUTTON_START)) {
		screen_transition_next(GAME_SCREEN_PLAYERTURN, SCREEN_TRANSITION_NONE, 0);
		game_reset_scores();
 		game_round_initialize();
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