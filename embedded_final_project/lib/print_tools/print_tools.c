/*
 * print_tools.c
 *
 * Created: 12/1/2015 3:46:05 PM
 *  Author: Harrison
 */ 

#include <stdio.h>
#include "print_tools.h"

// Clears the entire screen
void pt_clear_screen() {
	printf("\e[2J\e[H");
}

// Sends a color code
void pt_set_color_modifier(unsigned char mod) {
	printf("\e[%dm", mod);
}
// Resets all color codes
void pt_clear_colors() {
	printf("\e[0m");
}

// Get the opposite color code of the provided color
unsigned char pt_get_opposite_color(unsigned char color) {
	if (color >= 30 && color <= 37) {
		return color + 60;
		} else if (color >= 90 && color <= 97) {
		return color - 60;
		} else {
		return color;
	}
}

void pt_set_cursor_pos(unsigned char x, unsigned char y) {
	printf("\e[%u;%uH", y + 1, x + 1);
}

void pt_move_cursor_x(char offset) {
	if (offset < 0) {
		printf("\e[%u;D", offset);
	} else {
		printf("\e[%u;C", offset);
	}
}

void pt_move_cursor_y(char offset) {
	if (offset < 0) {
		printf("\e[%u;A", offset);
		} else {
		printf("\e[%u;B", offset);
	}
}

void pt_save_cursor_pos() {
	printf("\e[s");
}
void pt_restore_cursor_pos() {
	printf("\e[u");
}