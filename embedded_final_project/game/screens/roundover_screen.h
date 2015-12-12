/*
 * roundover_screen.h
 *
 * Created: 12/12/2015 2:41:45 PM
 *  Author: Harrison
 */ 


#ifndef ROUNDOVER_SCREEN_H_
#define ROUNDOVER_SCREEN_H_

#define ROUNDOVER_SCREEN_TICKER_SPEED GAME_PAGE_SCROLL_SPEED
#define ROUNDOVER_SCREEN_NUMBER_PAGES 3

void roundover_init();
void roundover_begin();
void roundover_render(char* buffer);
void roundover_update();
void roundover_stop();
void roundover_destroy();



#endif /* ROUNDOVER_SCREEN_H_ */