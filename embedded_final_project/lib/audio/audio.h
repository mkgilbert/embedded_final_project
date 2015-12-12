/*
 * audio.h
 *
 * Created: 12/8/2015 11:43:20 PM
 *  Author: Brandon Garling
 */ 


#ifndef AUDIO_H_
#define AUDIO_H_

#include "lib/fat32/fat32.h"

// Buffer information
#define AUDIO_BUFFER_SIZE 512
#define AUDIO_HALF_BUFFER_SIZE AUDIO_BUFFER_SIZE/2

// Track information
#define AUDIO_MAX_TRACKS 3
#define AUDIO_TRACK_0 0
#define AUDIO_TRACK_1 1
#define AUDIO_TRACK_2 2

// WAV file header information
#define AUDIO_WAV_CHUNK_ID_VAL 0x46464952
#define AUDIO_WAV_CHUNK_ID_POS 0

#define AUDIO_WAV_FORMAT_VAL 0x45564157
#define AUDIO_WAV_FORMAT_POS 8

#define AUDIO_WAV_SAMPLE_RATE_POS 24

typedef struct _audio_file {
	fat32_file_t file;
	uint32_t sample_rate;
	uint32_t sample_offset;
	uint8_t is_playing;
	uint8_t loop;
	uint8_t last_loaded;
	volatile uint32_t sample_position;
	volatile uint8_t buffer[AUDIO_BUFFER_SIZE];
} audio_file_t;

void audio_init(void);
void audio_play(char* fname, uint8_t loop, uint8_t track);
void audio_update();
void audio_pause(uint8_t track);
void audio_resume(uint8_t track);
void audio_stop(uint8_t track);
uint8_t audio_track_is_playing(uint8_t track);

#endif /* AUDIO_H_ */