/*
 * begin_screen.h
 *
 * Created: 12/10/2015 9:09:54 PM
 *  Author: Harrison
 */ 


#ifndef BEGIN_SCREEN_H_
#define BEGIN_SCREEN_H_

#define BEGIN_SCREEN_TICKER_SPEED GAME_PAGE_SCROLL_SPEED
#define BEGIN_SCREEN_NUMBER_PAGES 2

void begin_init();
void begin_begin();
void begin_render(char* buffer);
void begin_update();
void begin_stop();
void begin_destroy();

#endif /* BEGIN_SCREEN_H_ */