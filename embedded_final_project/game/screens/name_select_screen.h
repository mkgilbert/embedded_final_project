/*
 * name_select_screen.h
 *
 * Created: 12/12/2015 7:04:41 PM
 *  Author: Brandon
 */ 


#ifndef NAME_SELECT_SCREEN_H_
#define NAME_SELECT_SCREEN_H_

void name_select_init();
void name_select_begin();
void name_select_render(char* buffer);
void name_select_update();
void name_select_stop();
void name_select_destroy();

#endif /* NAME_SELECT_SCREEN_H_ */