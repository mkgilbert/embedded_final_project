/*
 * screen.c
 *
 * Created: 12/10/2015 9:29:45 PM
 *  Author: Harrison
 */ 

#include <stdio.h>
#include <string.h>
#include "screen.h"
#include "game_internals.h"

// The current screen being rendered and updated
Screen current_screen = {0};

// The next screen to transition to
Screen next_screen = {0};

// Transition information
uint8_t current_transition = SCREEN_TRANSITION_NONE;
uint16_t transition_speed = 0;
uint16_t transition_pos = 0;
uint8_t is_transitioning = 0;

// Display buffers
char current_screen_buffer[32]; // The current screen's render buffer
char next_screen_buffer[32];    // The next screen's render buffer - for transitions
char display_buffer[32];        // The temp display buffer - for transitions - will use both the current and next screen buffers to generate this
char display_old_buffer[32];    // The data displayed last frame

// Internal function prototypes
void screen_advance();
void _screen_draw_buffer(char* buffer, char* oldbuffer);

// Initializes a screen
uint8_t screen_init(Screen* s) {
	
	switch (s->state) {
		case SCREEN_STATE_UNINITIALIZED:
			s->init();
			s->state = SCREEN_STATE_INITIALIZED;
			return 1;
	}
	
	return 0;
	
}

// Begins running a screen, and initializes it if needed
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

// Runs the update method on a screen, and initializes/begins it if needed
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

// Runs the render method on a screen, and initializes it if needed.
// The output will fill the provided 32 byte buffer
uint8_t screen_render(Screen* s, char* buffer) {
	
	switch (s->state) {
		case SCREEN_STATE_UNINITIALIZED:
			if (!screen_init(s))
				return 0;
		case SCREEN_STATE_INITIALIZED:
		case SCREEN_STATE_RUNNING:
		case SCREEN_STATE_STOPPED:
			memset(buffer, ' ', 32);
			s->render(buffer);
			return 1;
	}
	
	return 0;
}

// Stops a screen
uint8_t screen_stop(Screen* s) {
	
	switch (s->state) {
		case SCREEN_STATE_RUNNING:
			s->stop();
			s->state = SCREEN_STATE_STOPPED;
			return 1;
	}
	
	return 0;
}

// Destroys a screen, stopping it if needed
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

// Sets the current screen to the provided screen and begins it
// will destroy the existing current screen
void _screen_set_current(Screen s) {
	
	screen_destroy(&current_screen);
	current_screen = s;
	screen_begin(&current_screen);
}

// Sets the next screen to the provided screen and initializes it
// will destroy the next screen if needed
void _screen_set_next(Screen s) {
	
	screen_destroy(&next_screen);
	next_screen = s;
	screen_init(&next_screen);
}

// Does one render and update cycle.
// Progresses any active transitions
// Updates the currently running screen
// Renders the currently running screen
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
		// The update may have ended the screen, so check again
		if (current_screen.state == SCREEN_STATE_RUNNING) {
			screen_render(&current_screen, current_screen_buffer);
			_screen_draw_buffer(current_screen_buffer, display_old_buffer);
		}
	}
}

// Compares the passed buffer to the passed oldbuffer and draws
// any lines that have been changed.
// Afterwards, the buffer is copied into oldbuffer
void _screen_draw_buffer(char* buffer, char* oldbuffer) {
	uint8_t i;
	
	// Check the first line
	uint8_t diff_found = 0;
	for (i = 0; i < 16; i++) {
		if (buffer[i] != oldbuffer[i]) {
			diff_found = 1;
			break;
		}
	}
	if (diff_found) {
		game_print_lcd(0, 0, buffer);
		memcpy(oldbuffer, buffer, 16);
	}
	
	// Check the second line
	diff_found = 0;
	for (i = 16; i < 32; i++) {
		if (buffer[i] != oldbuffer[i]) {
			diff_found = 1;
			break;
		}
	}
	if (diff_found) {
		game_print_lcd(0, 1, buffer+16);
		memcpy(oldbuffer+16, buffer+16, 16);
	}
}

// Immediately advances to the next screen, destroying the current screen
void screen_advance() {
	// Destroy the current screen
	screen_destroy(&current_screen);
	
	// Copy the next screen to the current
	current_screen = next_screen;
	
	// Delete the next screen
	Screen x = {0};
	next_screen = x;
	
	// Begin the current screen, and render it
	screen_begin(&current_screen);
	screen_render(&current_screen, current_screen_buffer);
}

// Begins a transition to the next screen at the specified screen.
// Stops the current screen from running, but does not destroy it.
// When the transition is complete, the screen will be advanced to the next
void screen_transition(uint8_t transition, uint16_t speed) {
	screen_stop(&current_screen);
	
	current_transition = transition;
	transition_speed = speed;
	transition_pos = 0;
	is_transitioning = 1;
}