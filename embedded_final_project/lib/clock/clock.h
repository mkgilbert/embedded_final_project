/*
 * clock.h
 *
 * Created: 11/4/2015 9:31:44 PM
 *  Author: Harrison
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_


void clock_init(void);
void clock_pause(void);
unsigned long long clock_get_ms(void);


#endif /* CLOCK_H_ */