/*
 * turn_screen.c
 *
 * Created: 12/10/2015 10:27:22 PM
 *  Author: Harrison
 */ 

#include <stdint.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "turn_screen.h"
#include "roundover_screen.h"
#include "gameover_screen.h"
#include "playback_screen.h"
#include "../screen.h"
#include "../game_internals.h"
#include "lib/tasks/tasks.h"
#include "lib/audio/audio.h"
#include "lib/kb/kb.h"

uint8_t turn_seconds_remaining = 0, turn_timeout_task = 0, turn_turn_count = 0;
uint8_t turn_feedback_task = 0, turn_feedback_progress = 0;

void turn_decrease_timer();
void turn_continue_delay();

void turn_incorrect_delay();
void turn_correct_delay();

void turn_init() {
	turn_seconds_remaining = TURN_SCREEN_PLAY_TIME / 1000;
	turn_turn_count = 0;
	turn_feedback_progress = 0;
}

void turn_begin() {
	turn_timeout_task = task_create(turn_decrease_timer, 1000, 1);
	game_set_leds(GAME_NO_COLORS);
}

void turn_decrease_timer() {
	turn_seconds_remaining--;
}

void turn_continue_delay() {
	turn_feedback_task = task_create(turn_correct_delay, TURN_FEEDBACK_ANIMATION_CORRECT_SPEED, 1);
}

void turn_incorrect_delay() {
	if (turn_feedback_progress < TURN_FEEDBACK_ANIMATION_INCORRECT_COUNT) {
		game_set_leds(turn_feedback_progress++ % 2 ? GAME_ALL_COLORS: GAME_NO_COLORS);
	} else {
		task_delete(turn_feedback_task);
		game_set_winner(game_get_turn() == GAME_PLAYER_1 ? GAME_PLAYER_2 : GAME_PLAYER_1);
		if (game_is_score_limit_reached()) {
			screen_transition_next(GAME_SCREEN_GAMEOVER, SCREEN_TRANSITION_NONE, 0);
		}
		else {
			screen_transition_next(GAME_SCREEN_ROUNDOVER, SCREEN_TRANSITION_NONE, 0);
		}
	}
}
void turn_correct_delay() {
	if (turn_feedback_progress < TURN_FEEDBACK_ANIMATION_CORRECT_COUNT) {
		game_set_leds(turn_feedback_progress++ % 2 ? GAME_ALL_COLORS : GAME_NO_COLORS);
	} else {
		task_delete(turn_feedback_task);
		game_advance_turn();
		screen_transition_next(GAME_SCREEN_PLAYBACK, SCREEN_TRANSITION_NONE, 0);
	}
}

void turn_render(char* buffer) {
	game_print_buffer(buffer, 0, 0, " Player X Turn  ");
	game_print_buffer(buffer, 8, 0, game_get_turn() == GAME_PLAYER_1 ? GAME_PLAYER_1_STR : GAME_PLAYER_2_STR);
	char remaining[3];
	itoa(turn_seconds_remaining, remaining, 10);
	game_print_buffer(buffer, 7, 1, remaining);
}

void turn_update() {
	
	uint8_t total_turns = game_get_move_count();
	
	if (turn_turn_count > total_turns) {
		return;
	}
	
	if (turn_seconds_remaining == 0) {
		turn_feedback_task = task_create(turn_incorrect_delay, TURN_FEEDBACK_ANIMATION_INCORRECT_SPEED, 1);
	}
	
	uint8_t move = GAME_MOVE_NONE;
	
	// TODO: Use game_button_is_down
	/*
	if (kb_haschar()) {
		char input = kb_getchar();
		switch (input) {
			case GAME_BUTTON_RED: move = GAME_MOVE_RED; break;
			case GAME_BUTTON_BLUE: move = GAME_MOVE_BLUE; break;
			case GAME_BUTTON_GREEN: move = GAME_MOVE_GREEN; break;
			case GAME_BUTTON_YELLOW: move = GAME_MOVE_YELLOW; break;
		}
	}*/
	
	if (game_button_is_down(game_get_turn(), GAME_BUTTON_RED)) {move = GAME_MOVE_RED;}
	else if (game_button_is_down(game_get_turn(), GAME_BUTTON_BLUE)) {move = GAME_MOVE_BLUE;}
	else if (game_button_is_down(game_get_turn(), GAME_BUTTON_GREEN)) {move = GAME_MOVE_GREEN;}
	else if (game_button_is_down(game_get_turn(), GAME_BUTTON_YELLOW)) {move = GAME_MOVE_YELLOW;}
	
	if (move != GAME_MOVE_NONE) {
		audio_play("sndButton01_low.wav", 0, AUDIO_TRACK_2);
		if (move != GAME_MOVE_NONE) {
			game_set_led(move);
		}
		if (turn_turn_count < total_turns) {
			// Check if the turn is correct
			uint8_t check = game_get_move(turn_turn_count++);
			if (check != move) {
				turn_feedback_task = task_create(turn_incorrect_delay, TURN_FEEDBACK_ANIMATION_INCORRECT_SPEED, 1);
			} else {
				turn_seconds_remaining = TURN_SCREEN_PLAY_TIME / 1000;
				task_reset(turn_timeout_task);
			}
		} else if (turn_turn_count == total_turns) {
			// Add a new turn
			game_set_move(move, turn_turn_count++);
			task_create(turn_continue_delay, TURN_SCREEN_CONTINUE_DELAY, 0);
		}
	}
	
}

void turn_stop() {
	task_delete(turn_timeout_task);
}

void turn_destroy() {
	
}