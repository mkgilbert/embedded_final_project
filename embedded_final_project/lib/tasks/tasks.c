/*
 * tasks.c
 *
 * Created: 11/4/2015 9:31:58 PM
 *  Author: Harrison
 */ 

#include "tasks.h"
#include "lib/clock/clock.h"

// The list of available tasks
TASK tasks[NUM_TASKS];
uint8_t num_tasks = 0;
uint8_t last_id = 1;

// Creates a new task and returns the UID of the newly created task.
// Returns 0 if no task could be created
uint8_t task_create(void (*task)(), uint64_t interval, uint8_t repeat) {
	
	if (!clock_is_enabled()) {
		clock_init();
	}
	
	if (num_tasks == NUM_TASKS)
		return 0;
	
	// Create the new task
	TASK* t = &tasks[num_tasks++];
	t->created = clock_get_ms();
	t->repeat = repeat;
	t->interval = interval;
	t->repeat = repeat;
	t->next_expiration = t->created + t->interval;
	t->task = task;
	t->uid = last_id++;
	
	// Return the uid of the task
	return t->uid;
	
}

// Update all running tasks
void task_update() {
	
	// Get the current time
	uint64_t time = clock_get_ms();
	
	// Iterate over all of the tasks
	uint8_t i;
	TASK* t;
	for(i = 0; i < num_tasks; i++) {
		t = &tasks[i];
		
		// Check if the t ask is expired
		if (time >= t->next_expiration) {
			
			// Run the task
			t->task();
			
			if (t->repeat) {
				// Set the next expiration time
				t->next_expiration += t->interval;
			} else {
				// Delete the task, it isn't going to repeat
				task_delete(t->uid);
			}
		}
	}
}

// Reset a running task's timer
void task_reset(uint8_t id) {
	// Iterate over the tasks to find the specified task
	uint8_t i;
	TASK* t;
	for (i = 0; i < num_tasks; i++) {
		t = &tasks[i];
		if (t->uid == id) {
			t->next_expiration = clock_get_ms() + t->interval;
			return;
		}
	}
}

// Delete a task with the specified ID
void task_delete(uint8_t id) {
	
	// Iterate over the tasks to find the specified task
	uint8_t i;
	int8_t found_index = -1;
	TASK* t;
	for (i = 0; i < num_tasks; i++) {
		t = &tasks[i];
		if (t->uid == id) {
			found_index = i;
			break;
		}
	}
	
	// If the task was found, move all created tasks left to delete the task
	if (found_index >= 0) {
		for (i = found_index+1; i < num_tasks; i++) {
			tasks[i-1] = tasks[i];
		}
		num_tasks--;
	}
}
