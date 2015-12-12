/*
 * gameover_screen.h
 *
 * Created: 12/12/2015 2:42:01 PM
 *  Author: Harrison
 */ 


#ifndef GAMEOVER_SCREEN_H_
#define GAMEOVER_SCREEN_H_

#define GAMEOVER_SCREEN_TICKER_SPEED GAME_PAGE_SCROLL_SPEED
#define GAMEOVER_SCREEN_NUMBER_PAGES 3

void gameover_init();
void gameover_begin();
void gameover_render(char* buffer);
void gameover_update();
void gameover_stop();
void gameover_destroy();

#endif /* GAMEOVER_SCREEN_H_ */