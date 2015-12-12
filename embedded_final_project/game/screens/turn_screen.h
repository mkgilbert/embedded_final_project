/*
 * turn_screen.h
 *
 * Created: 12/10/2015 10:27:32 PM
 *  Author: Harrison
 */ 


#ifndef TURN_SCREEN_H_
#define TURN_SCREEN_H_

#define TURN_SCREEN_PLAY_TIME GAME_MOVE_TIMEOUT

void turn_init();
void turn_begin();
void turn_render(char* buffer);
void turn_update();
void turn_stop();
void turn_destroy();

#endif /* TURN_SCREEN_H_ */