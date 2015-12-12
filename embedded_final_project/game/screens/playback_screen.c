/*
 * playback_sceren.c
 *
 * Created: 12/12/2015 1:20:17 PM
 *  Author: Harrison
 */ 

#include <stdint.h>
#include "../game_internals.h"
#include "playback_screen.h"

void playback_init() {
	
}

void playback_begin() {
	
}

void playback_render(char* buffer) {
	game_print_buffer(buffer, 0, 0, "    PLAYER X    ");
	game_print_buffer(buffer, 0, 1, "   GET READY!   ");
	game_print_buffer(buffer, 11, 0, game_get_turn() == GAME_PLAYER_1 ? GAME_PLAYER_1_STR : GAME_PLAYER_2_STR);
}

void playback_update() {
	
}

void playback_stop() {
	
}

void playback_destroy() {
	
}