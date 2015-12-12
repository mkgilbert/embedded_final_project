/*
 * playback_sceren.c
 *
 * Created: 12/12/2015 1:20:17 PM
 *  Author: Harrison
 */ 

#include <stdint.h>
#include "../game_internals.h"
#include "../screen.h"
#include "lib/tasks/tasks.h"
#include "playback_screen.h"
#include "turn_screen.h"

uint8_t playback_turn_count = 0, playback_task = 0;

void advance_playback();

void playback_init() {
	playback_turn_count = 0;
}

void playback_begin() {
	playback_task = task_create(advance_playback, PLAYBACK_SCREEN_DELAY, 1);
}

void playback_render(char* buffer) {
	game_print_buffer(buffer, 0, 0, "    PLAYER X    ");
	game_print_buffer(buffer, 0, 1, "   GET READY!   ");
	game_print_buffer(buffer, 11, 0, game_get_turn() == GAME_PLAYER_1 ? GAME_PLAYER_1_STR : GAME_PLAYER_2_STR);
}

void playback_update() {
	
	if (playback_turn_count >= game_get_move_count()) {
		screen_transition_next(GAME_SCREEN_PLAYERTURN, SCREEN_TRANSITION_NONE, 0);
	} else {
		game_set_led(game_get_move(playback_turn_count));
	}
	
}

void playback_stop() {
	task_delete(playback_task);
}

void playback_destroy() {
	
}

void advance_playback() {
	playback_turn_count++;
}