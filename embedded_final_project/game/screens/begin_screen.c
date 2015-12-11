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

uint8_t ticker_task, page = 0, request_render = 0;

void begin_init() {
	page = 0;
	request_render = 1;
}

void begin_begin() {
	ticker_task = task_create(ticker_tick, BEGIN_SCREEN_TICKER_SPEED, 1);
}

void begin_render() {
	if (!request_render)
		return;
	
	switch (page) {
		case 0:
			game_print_scores();
			break;
		case 1:
			game_print_lcd(0, 0, "Welcome to Simon");
			game_print_lcd(0, 1, " P1 Press Start ");
			break;
	}
	request_render = 0;
}

void begin_update() {
	if (game_button_is_down(GAME_PLAYER_1, GAME_BUTTON_START)) {
		screen_transition_next(GAME_SCREEN_PLAYERTURN, SCREEN_TRANSITION_NONE, 0);
		//Screen next = {0};
		//screen_fill(next, turn);
// 		game_p1_score = 0;
// 		game_p2_score = 0;
// 		game_round_initialize();
	}
}

void begin_stop() {
	task_delete(ticker_task);
}

void begin_destroy() {
	
}


void ticker_tick() {
	page = ++page % BEGIN_SCREEN_NUMBER_PAGES;
	request_render = 1;
}