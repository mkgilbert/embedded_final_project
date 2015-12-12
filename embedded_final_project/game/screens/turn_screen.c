/*
 * turn_screen.c
 *
 * Created: 12/10/2015 10:27:22 PM
 *  Author: Harrison
 */ 

#include <stdint.h>
#include <stdlib.h>
#include "turn_screen.h"
#include "begin_screen.h"
#include "playback_screen.h"
#include "../screen.h"
#include "../game_internals.h"
#include "lib/tasks/tasks.h"
#include "lib/kb/kb.h"

uint8_t seconds_remaining = 0, timeout_task = 0, turn_count = 0;

void decrease_timer();

void turn_init() {
	seconds_remaining = TURN_SCREEN_PLAY_TIME / 1000;
	turn_count = 0;
}

void turn_begin() {
	timeout_task = task_create(decrease_timer, 1000, 1);
}

void decrease_timer() {
	seconds_remaining--;
}

void turn_render(char* buffer) {
	game_print_buffer(buffer, 0, 0, " Player X Turn  ");
	game_print_buffer(buffer, 8, 0, game_get_turn() == GAME_PLAYER_1 ? GAME_PLAYER_1_STR : GAME_PLAYER_2_STR);
	char remaining[3];
	itoa(seconds_remaining, remaining, 10);
	game_print_buffer(buffer, 7, 1, remaining);
}

void turn_update() {
	
	uint8_t total_turns = game_get_move_count();
	
	if (seconds_remaining == 0) {
		screen_transition_next(GAME_SCREEN_BEGIN, SCREEN_TRANSITION_NONE, 0);
	}
	
	uint8_t move = GAME_MOVE_NONE;
	
	// TODO: Use game_button_is_down
	if (kb_haschar()) {
		char input = kb_getchar();
		switch (input) {
			case GAME_BUTTON_RED: move = GAME_MOVE_RED; break;
			case GAME_BUTTON_BLUE: move = GAME_MOVE_BLUE; break;
			case GAME_BUTTON_GREEN: move = GAME_MOVE_GREEN; break;
			case GAME_BUTTON_YELLOW: move = GAME_MOVE_YELLOW; break;
		}
	}
	
	//if (game_button_is_down(game_turn, GAME_BUTTON_RED)) {move = GAME_MOVE_RED;}
	//else if (game_button_is_down(game_turn, GAME_BUTTON_BLUE)) {move = GAME_MOVE_BLUE;}
	//else if (game_button_is_down(game_turn, GAME_BUTTON_GREEN)) {move = GAME_MOVE_GREEN;}
	//else if (game_button_is_down(game_turn, GAME_BUTTON_YELLOW)) {move = GAME_MOVE_YELLOW;}
	
	if (move != GAME_MOVE_NONE) {
		if (move != GAME_MOVE_NONE) {
			game_set_led(move);
		}
		if (turn_count < total_turns) {
			// Check if the turn is correct
			uint8_t check = game_get_move(turn_count++);
			if (check != move) {
				screen_transition_next(GAME_SCREEN_BEGIN, SCREEN_TRANSITION_NONE, 0);
			}
		} else if (turn_count == total_turns) {
			// Add a new turn
			game_set_move(move, turn_count++);
			game_advance_turn();
			screen_transition_next(GAME_SCREEN_PLAYBACK, SCREEN_TRANSITION_NONE, 0);
		}
	}
	
}

void turn_stop() {
	task_delete(timeout_task);
}

void turn_destroy() {
	
}