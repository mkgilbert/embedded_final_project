/*
 * kb.h
 *
 * Created: 12/10/2015 12:48:03 AM
 *  Author: Harrison
 */ 


#ifndef KB_H_
#define KB_H_

#include <stdint.h>

#define KB_DATA_PORT B
#define KB_DATA_PIN 1
#define KB_CLK_PORT B
#define KB_CLK_PIN 0
#define KB_INTERRUPT PCINT8
#define KB_PCIE PCIE1
#define KB_PCMSK PCMSK1
#define KB_PCINT_VECT PCINT1_vect

void kb_init();
unsigned char kb_getchar();
uint8_t kb_haschar();

#endif /* KB_H_ */