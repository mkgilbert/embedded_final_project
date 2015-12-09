/*
 * audio.h
 *
 * Created: 12/8/2015 11:43:20 PM
 *  Author: Brandon Garling
 */ 


#ifndef AUDIO_H_
#define AUDIO_H_

#include "lib/fat32/fat32.h"

#define BUFFER_SIZE 256
#define HALF_BUFFER_SIZE BUFFER_SIZE/2

void audio_init(void);
void audio_play(fat32_file_t * file);
void audio_step();


#endif /* AUDIO_H_ */