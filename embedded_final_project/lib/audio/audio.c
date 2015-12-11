/*
 * audio.c
 *
 * Created: 12/8/2015 11:43:11 PM
 *  Author: Brandon Garling
 */ 


#include "lib/audio/audio.h"
#include "lib/fat32/fat32.h"
#include "system.h"
#include <avr/interrupt.h>
#include <stdio.h>

volatile audio_file_t audio_files[AUDIO_MAX_TRACKS];

/************************************************************************/
/* audio_init()                                             
   Initializes the audio module.
   
   This module relies on:
      -FAT32
	  -SDCARD
	  -SPI
   This module makes use of:
      -TIMER 1
	  -TIMER 2
/************************************************************************/
void audio_init(void){
    /* use OC1A pin as output */
    DDRD = _BV(PD5);

    /* 
    * clear OC1A on compare match 
    * set OC1A at BOTTOM, non-inverting mode
    * Fast PWM, 8bit
    */
    TCCR1A = _BV(COM1A1) | _BV(WGM10);
    
    /* 
    * Fast PWM, 8bit
    * Prescaler: clk/8 = 3.575MHz
    * PWM frequency = 8MHz / (255 + 1) = 31.25kHz
    */
    TCCR1B = _BV(WGM10) | _BV(CS10);
    
    /* set initial duty cycle to zero */
    OCR1A = 0;
    
    /* Setup Timer0 */
  
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << CS21);

    TCNT2 = 0; // Start counter at 0
  
    // Initialize Compare Value
    OCR2A = 255;
    
    // Enable Compare Interrupt
    TIMSK2 = (1 << OCIE2A);
  
    sei(); //Enable interrupts
  
	audio_files[0].is_playing = FALSE;
	audio_files[1].is_playing = FALSE;
	audio_files[2].is_playing = FALSE;
}

/************************************************************************/
/* audio_play(char* fname, uint8_t loop, uint8_t track)                                             
   This sets up and loads an WAV audio file into the selected track.
   The file will begin playing as soon as it is set up by this function.
/************************************************************************/
void audio_play(char* fname, uint8_t loop, uint8_t track){
    fat32_file_init(&audio_files[track].file);
	fat32_file_lookup(&audio_files[track].file, fname);
	
	if (audio_files[track].file.state == FAT32_STATE_EMPTY)
		return;
	
	// Read the WAV file header information and ensure that it is a valid
	uint8_t buffer[44];
	fat32_read_file_data(&audio_files[track].file, buffer, 44, 0);
	
	if (fat32_parse_uint32(buffer + AUDIO_WAV_CHUNK_ID_POS) != AUDIO_WAV_CHUNK_ID_VAL ||
		fat32_parse_uint32(buffer + AUDIO_WAV_FORMAT_POS) != AUDIO_WAV_FORMAT_VAL){
		// This is not a valid WAV file, clear the file information from the track
		// and return.
		fat32_file_init(&audio_files[track].file);
		return;
	}
	
	audio_files[track].loop = loop;
	audio_files[track].is_playing = TRUE;
	audio_files[track].sample_position = 44;
	audio_files[track].sample_offset = 0;
	audio_files[track].sample_rate = fat32_parse_uint32(buffer+AUDIO_WAV_SAMPLE_RATE_POS);
	audio_files[track].last_loaded = 0;

	OCR2A = ((F_CPU/8)/audio_files[track].sample_rate) - 1;
}

/************************************************************************/
/* audio_step()                                              
   This streams in audio from the SD card into the appropriate buffers.
   This should be called whenever possible to load the buffers with new
   data before it needs to be sampled. Currently this takes around 2ms per
   audio file playing. However, this will vary depending on the buffer size.
/************************************************************************/
void audio_step(){
  
	int8_t i;
  
	for(i=AUDIO_MAX_TRACKS;i>=0;i--){
    
		if (audio_files[i].is_playing == FALSE)
			continue;
    
		if ((audio_files[i].sample_offset + audio_files[i].sample_position) >= audio_files[i].file.size - 1){
			if (audio_files[i].loop){
				audio_files[i].sample_offset = 0;
				audio_files[i].sample_position = 0;
			} else {
				audio_files[i].is_playing = FALSE;
				continue;
			}
		}
		
		if (audio_files[i].sample_position < AUDIO_HALF_BUFFER_SIZE && audio_files[i].last_loaded == 0){
			audio_files[i].last_loaded = 1;
			audio_files[i].sample_offset += AUDIO_HALF_BUFFER_SIZE;
			fat32_read_file_data(&audio_files[i].file, (uint8_t*) audio_files[i].buffer + AUDIO_HALF_BUFFER_SIZE, AUDIO_HALF_BUFFER_SIZE, audio_files[i].sample_offset);
		}
		else if (audio_files[i].sample_position >= AUDIO_HALF_BUFFER_SIZE && audio_files[i].last_loaded == 1) {
			audio_files[i].last_loaded = 0;
			audio_files[i].sample_offset += AUDIO_HALF_BUFFER_SIZE;
			fat32_read_file_data(&audio_files[i].file, (uint8_t*) audio_files[i].buffer, AUDIO_HALF_BUFFER_SIZE, audio_files[i].sample_offset);
		}
	}
}

/************************************************************************/
/* audio_pause(uint8_t track)                                              
   Pauses an audio track without unloading the file. Tracks
   should use the AUDIO_TRACK_X format where X is a number
   between 0 and AUDIO_MAX_TRACKS - 1.
/************************************************************************/
void audio_pause(uint8_t track){
	audio_files[track].is_playing = FALSE;
}

/************************************************************************/
/* audio_resume(uint8_t track)                                              
   Resumes a paused audio track from its previous position.
   Tracks should use the AUDIO_TRACK_X format where X is a 
   number between 0 and AUDIO_MAX_TRACKS - 1.
/************************************************************************/
void audio_resume(uint8_t track){
	// Make sure there is a file in the track before we try to resume it.
	if (audio_files[track].file.state == FAT32_STATE_OK)
		audio_files[track].is_playing = TRUE;
}

/************************************************************************/
/* audio_stop(uint8_t track)                                              
   Stops and unloads the audio file playing on the selected audio track.
   After an audio file has been stopped you cannot resume, you must reload
   the file using the audio_play function. Tracks should use the AUDIO_TRACK_X 
   format where X is a number between 0 and AUDIO_MAX_TRACKS - 1.
/************************************************************************/
void audio_stop(uint8_t track){
	audio_files[track].is_playing = FALSE;
	fat32_file_init(&audio_files[track].file);
}

/************************************************************************/
/* audio_track_is_playing(uint8_t track)                                  
   Checks to see if there is a currently playing audio file playing on
   the selected audio track. Tracks should use the AUDIO_TRACK_X format 
   where X is a number between 0 and AUDIO_MAX_TRACKS - 1.
/************************************************************************/
uint8_t audio_track_is_playing(uint8_t track){
	return audio_files[track].is_playing;
}

/************************************************************************/
/* ISR (TIMER2_COMPA_vect)                                               
   This is the interrupt that will fire when a new sample should be 
   loaded into the OCR1A register. It supports mixing multiple audio
   sources (currently up to 3) at the same sample rate.
/************************************************************************/
ISR (TIMER2_COMPA_vect)
{
	int8_t i;
	uint16_t total = 0;
	
	for(i=AUDIO_MAX_TRACKS;i>=0;i--){
		if (audio_files[i].is_playing == FALSE)
			continue;
		
		if (audio_files[i].sample_position >= AUDIO_BUFFER_SIZE){
			audio_files[i].sample_position = 0;
		}
		total += audio_files[i].buffer[audio_files[i].sample_position++];
	}
	
	OCR1A = total/AUDIO_MAX_TRACKS;
}