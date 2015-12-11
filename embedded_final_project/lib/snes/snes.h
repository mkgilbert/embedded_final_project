/*
 * snes.h
 *
 * Created: 12/10/2015 3:36:42 PM
 *  Author: Matt Q
 */ 


#ifndef SNES_H_
#define SNES_H_

#define SNES_CLOCK_PIN 1
#define SNES_CLOCK_PORT A
#define SNES_LATCH_PIN 2
#define SNES_LATCH_PORT A
#define SNES_DATA1_PIN 3
#define SNES_DATA1_PORT A
#define SNES_DATA2_PORT A
#define SNES_DATA2_PIN 4

#define SNES_BTN_B 1 
#define SNES_BTN_Y 2
#define SNES_BTN_SELECT 3
#define SNES_BTN_START 4
#define SNES_BTN_UP 5
#define SNES_BTN_DOWN 6
#define SNES_BTN_LEFT 7
#define SNES_BTN_RIGHT 8
#define SNES_BTN_A 9
#define SNES_BTN_X 10
#define SNES_BTN_L 11
#define SNES_BTN_R 12

#define SNES_CTRL1 1
#define SNES_CTRL2 2

uint8_t snes_is_pressed(uint8_t controller, uint8_t button);
void snes_init();
void snes_update();

#endif /* SNES_H_ */