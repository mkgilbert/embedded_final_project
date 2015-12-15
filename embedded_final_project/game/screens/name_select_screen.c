/*
 * name_select_screen.c
 *
 * Created: 12/12/2015 7:04:24 PM
 *  Author: Brandon
 */ 

#include "name_select_screen.h"
#include "begin_screen.h"
#include "../screen.h"
#include "../game_internals.h"
#include "lib/lcd/lcd.h"
#include "lib/kb/kb.h"
#include "lib/audio/audio.h"

uint8_t name_select_char_pos;

void name_select_init() {
	name_select_char_pos = strlen(game_get_p_str(game_get_ns()));
}

void name_select_begin() {
	LCD_CURSOR_BLINK_ON();
	
}

void name_select_render(char* buffer) {
	
	game_print_buffer(buffer, 0, 0, "PX Name: ___");
	game_print_buffer(buffer, 1, 0, game_get_ns() == GAME_PLAYER_1 ? GAME_PLAYER_1_STR : GAME_PLAYER_2_STR);
	game_print_buffer(buffer, 9, 0, game_get_p_str(game_get_ns()));
	game_print_buffer(buffer, 0, 1, "Save Using Enter");
	uint8_t p = name_select_char_pos;
	if (p > 2)
		p = 2;
	lcd_set_cursor(9 + p, 0);
	
}

void name_select_update() {
	if (kb_haschar()) {
		char* name = game_get_p_str(game_get_ns());
		char c = kb_getchar();
		if (c == '\b' && name_select_char_pos > 0) {
			name[--name_select_char_pos] = 0;
		} else if (c == '\n') {
			screen_transition_next(GAME_SCREEN_BEGIN, SCREEN_TRANSITION_NONE, 0);
		} else if (c >= 32 && c <= 126 && name_select_char_pos < 3) {
			name[name_select_char_pos++] = c;
		}
	}
	
}

void name_select_stop() {
	LCD_CURSOR_BLINK_OFF();
	if (!strcmp(game_get_p_str(game_get_ns()), "JC")) {
		audio_play("sndJohnCena_low.wav", 0, AUDIO_TRACK_1);
	}
}

void name_select_destroy() {
	
}