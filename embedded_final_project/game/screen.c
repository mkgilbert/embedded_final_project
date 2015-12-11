/*
 * screen.c
 *
 * Created: 12/10/2015 9:29:45 PM
 *  Author: Harrison
 */ 

#include "screen.h"
#include "game_internals.h"

Screen current_screen = {0};
Screen next_screen = {0};

uint8_t current_transition = SCREEN_TRANSITION_NONE;
uint16_t transition_speed = 0;
uint16_t transition_pos = 0;
uint8_t is_transitioning = 0;

void screen_advance();

uint8_t screen_init(Screen* s) {
	
	switch (s->state) {
		case SCREEN_STATE_UNINITIALIZED:
			s->init();
			s->state = SCREEN_STATE_INITIALIZED;
			return 1;
	}
	
	return 0;
	
}

uint8_t screen_begin(Screen* s) {
	
	switch (s->state) {
		case SCREEN_STATE_UNINITIALIZED:
			if (!screen_init(s))
				return 0;
		case SCREEN_STATE_INITIALIZED:
			s->begin();
			s->state = SCREEN_STATE_RUNNING;
			return 1;
	}
	
	return 0;
}

uint8_t screen_update(Screen* s) {
	
	switch (s->state) {
		case SCREEN_STATE_UNINITIALIZED:
		case SCREEN_STATE_INITIALIZED:
			if (!screen_begin(s))
				return 0;
		case SCREEN_STATE_RUNNING:
			s->update();
			return 1;
	}
	
	return 0;
	
}

uint8_t screen_render(Screen* s) {
	
	switch (s->state) {
		case SCREEN_STATE_UNINITIALIZED:
			if (!screen_init(s))
				return 0;
		case SCREEN_STATE_INITIALIZED:
		case SCREEN_STATE_RUNNING:
		case SCREEN_STATE_STOPPED:
			s->render();
			return 1;
	}
	
	return 0;
}

uint8_t screen_stop(Screen* s) {
	
	switch (s->state) {
		case SCREEN_STATE_RUNNING:
			s->stop();
			s->state = SCREEN_STATE_STOPPED;
			return 1;
	}
	
	return 0;
}

uint8_t screen_destroy(Screen* s) {
	
	switch (s->state) {
		case SCREEN_STATE_RUNNING:
			if (!screen_stop(s))
				return 0;
		case SCREEN_STATE_STOPPED:
			s->destroy();
			s->state = SCREEN_STATE_UNINITIALIZED;
			return 1;
	}
	
	return 0;
}

void _screen_set_current(Screen s) {
	
	screen_destroy(&current_screen);
	current_screen = s;
	screen_begin(&current_screen);
}

void _screen_set_next(Screen s) {
	
	screen_destroy(&next_screen);
	next_screen = s;
	screen_init(&next_screen);
}

void screen_tick() {
	
	if (is_transitioning) {
		
		switch (current_transition) {
			case SCREEN_TRANSITION_NONE:
			default:
				is_transitioning = 0;
				screen_advance();
				break;
		}
		
	} else if (current_screen.state == SCREEN_STATE_RUNNING) {
		screen_update(&current_screen);
		if (current_screen.state == SCREEN_STATE_RUNNING) {
			screen_render(&current_screen);
		}
	}
}

void screen_advance() {
	screen_destroy(&current_screen);
	current_screen = next_screen;
	Screen x = {0};
	next_screen = x;
	game_clear_screen();
	screen_begin(&current_screen);
	screen_render(&current_screen);
}

void screen_transition(uint8_t transition, uint16_t speed) {
	screen_stop(&current_screen);
	
	current_transition = transition;
	transition_speed = speed;
	transition_pos = 0;
	is_transitioning = 1;
}