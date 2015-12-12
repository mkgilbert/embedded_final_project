/*
 * playback_screen.h
 *
 * Created: 12/12/2015 1:21:05 PM
 *  Author: Harrison
 */ 


#ifndef PLAYBACK_SCREEN_H_
#define PLAYBACK_SCREEN_H_

#define PLAYBACK_SCREEN_DELAY GAME_PLAYBACK_DELAY

void playback_init();
void playback_begin();
void playback_render(char* buffer);
void playback_update();
void playback_stop();
void playback_destroy();

#endif /* PLAYBACK_SCREEN_H_ */