/*
 * turn_screen.h
 *
 * Created: 12/10/2015 10:27:32 PM
 *  Author: Harrison
 */ 


#ifndef TURN_SCREEN_H_
#define TURN_SCREEN_H_

#define TURN_SCREEN_PLAY_TIME GAME_MOVE_TIMEOUT
#define TURN_SCREEN_CONTINUE_DELAY GAME_PLAYBACK_DELAY
#define TURN_FEEDBACK_ANIMATION_CORRECT_SPEED 200
#define TURN_FEEDBACK_ANIMATION_INCORRECT_SPEED 1500
#define TURN_FEEDBACK_ANIMATION_CORRECT_COUNT 7
#define TURN_FEEDBACK_ANIMATION_INCORRECT_COUNT 3

void turn_init();
void turn_begin();
void turn_render(char* buffer);
void turn_update();
void turn_stop();
void turn_destroy();

#endif /* TURN_SCREEN_H_ */