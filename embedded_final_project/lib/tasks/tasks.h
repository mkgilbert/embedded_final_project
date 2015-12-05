/*
 * tasks.h
 *
 * Created: 11/4/2015 9:32:06 PM
 *  Author: Harrison
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#define NUM_TASKS 20

typedef struct {
	unsigned char uid;          // Unique ID
	unsigned long long created; // Creation time
	unsigned long long next_expiration; // Next expiration time
	unsigned int interval;      // How often executed in ms
	unsigned char repeat;       // Repeat or no?
	void (*task)();             // Function to perform task
} TASK;

unsigned char task_create(void (*task)(), unsigned int interval, unsigned char repeat);
void task_update();
void task_delete(unsigned char id);

#endif /* TASKS_H_ */