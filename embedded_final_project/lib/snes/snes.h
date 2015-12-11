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

#define SNES_BTN_B 0
#define SNES_BTN_Y 1
#define SNES_BTN_SELECT 2
#define SNES_BTN_START 3
#define SNES_BTN_UP 4
#define SNES_BTN_DOWN 5
#define SNES_BTN_LEFT 6
#define SNES_BTN_RIGHT 7
#define SNES_BTN_A 8
#define SNES_BTN_X 9
#define SNES_BTN_L 10
#define SNES_BTN_R 11

#define SNES_CONTROLLER1 1
#define SNES_CONTROLLER2 2

uint8_t snes_is_pressed(uint8_t controller, uint8_t button);
void snes_init();
void snes_update();

#endif /* SNES_H_ */