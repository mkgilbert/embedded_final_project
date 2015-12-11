/*
 * screen.h
 *
 * Created: 12/10/2015 9:05:50 PM
 *  Author: Harrison
 */ 


#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>

// Defines
#define SCREEN_STATE_UNINITIALIZED 0
#define SCREEN_STATE_INITIALIZED 1
#define SCREEN_STATE_RUNNING 2
#define SCREEN_STATE_STOPPED 3

#define SCREEN_TRANSITION_NONE 0

// Macros
#define screen_fill(screen, func_prefix) _screen_fill(screen, func_prefix)
#define _screen_fill(screen, func_prefix) screen.init = func_prefix ## _init; \
                                          screen.begin = func_prefix ## _begin; \
										  screen.update = func_prefix ## _update; \
										  screen.render = func_prefix ## _render; \
										  screen.stop = func_prefix ## _stop; \
									 	  screen.destroy = func_prefix ## _destroy; \
									 	  screen.state = SCREEN_STATE_UNINITIALIZED;

#define screen_set_current(func_prefix) _set_current(func_prefix)
#define _set_current(func_prefix) Screen s = {0}; \
	                              _screen_fill(s, func_prefix); \
								  _screen_set_current(s);

#define screen_set_next(func_prefix) _set_next(func_prefix)
#define _set_next(func_prefix) Screen s = {0}; \
	                           _screen_fill(s, func_prefix); \
							   _screen_set_next(s);

#define screen_transition_next(func_prefix, transition, speed) _transition_next(func_prefix, transition, speed)
#define _transition_next(func_prefix, transition, speed) _set_next(func_prefix); \
                                                         screen_transition(transition, speed);
										 

// Screen struct
typedef struct {
	void (*init)();
	void (*begin)();
	void (*update)();
	void (*render)(char* buffer);
	void (*stop)();
	void (*destroy)();
	uint8_t state;
} Screen;

// Functions
uint8_t screen_init(Screen* s);
uint8_t screen_begin(Screen* s);
uint8_t screen_render(Screen* s, char* buffer);
uint8_t screen_update(Screen* s);
uint8_t screen_stop(Screen* s);
uint8_t screen_destroy(Screen* s);

void screen_tick();

void _screen_set_current(Screen s);
void _screen_set_next(Screen s);

void screen_transition(uint8_t transition, uint16_t speed);
void screen_advance();

#endif /* SCREEN_H_ */