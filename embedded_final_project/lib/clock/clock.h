/*
 * clock.h
 *
 * Created: 11/4/2015 9:31:44 PM
 *  Author: Harrison
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

void clock_init(void);
void clock_pause(void);
uint64_t clock_get_ms(void);
uint8_t clock_is_enabled(void);


#endif /* CLOCK_H_ */