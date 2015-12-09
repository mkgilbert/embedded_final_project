/*
 * tasks.h
 *
 * Created: 11/4/2015 9:32:06 PM
 *  Author: Harrison
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>

#define NUM_TASKS 5

typedef struct {
	uint8_t uid;                 // Unique ID
	uint64_t created;            // Creation time
	uint64_t next_expiration;    // Next expiration time
	uint64_t interval;           // How often executed in ms
	uint8_t repeat;              // Repeat or no?
	void (*task)();              // Function to perform task
} TASK;

uint8_t task_create(void (*task)(), uint64_t interval, uint8_t repeat);
void task_update();
void task_delete(uint8_t id);
void task_reset(uint8_t id);

#endif /* TASKS_H_ */