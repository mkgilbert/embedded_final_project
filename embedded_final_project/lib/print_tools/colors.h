/*
 * colors.h
 *
 * Created: 10/20/2015 12:23:19 AM
 *  Author: Harrison
 */ 


#ifndef COLORS_H_
#define COLORS_H_

#define COLOR_RESET_ALL 0

#define COLOR_MOD_BOLD 1
#define COLOR_MOD_DIM 2
#define COLOR_MOD_UNDERLINE 4
#define COLOR_MOD_BLINK 5
#define COLOR_MOD_INVERT 7
#define COLOR_MOD_HIDE 8

#define COLOR_RESET_BOLD 21
#define COLOR_RESET_DIM 22
#define COLOR_RESET_UNDERLINE 24
#define COLOR_RESET_BLINK 25
#define COLOR_RESET_INVERT 27
#define COLOR_RESET_HIDE 28

#define COLOR_DEFAULT 39

#define COLOR_BLACK 30
#define COLOR_RED 31
#define COLOR_GREEN 32
#define COLOR_YELLOW 33
#define COLOR_BLUE 34
#define COLOR_MAGENTA 35
#define COLOR_CYAN 36
#define COLOR_LIGHT_GRAY 37

#define COLOR_DARK_GRAY 90
#define COLOR_LIGHT_RED 91
#define COLOR_LIGHT_GREEN 92
#define COLOR_LIGHT_YELLOW 93
#define COLOR_LIGHT_BLUE 94
#define COLOR_LIGHT_MAGENTA 95
#define COLOR_LIGHT_CYAN 96
#define COLOR_WHITE 97

#define BACK_OFFSET 10
#define FORE_OFFSET 0

// Get the terminal code for the foreground or background in a color.
#define BACK(COL) (COLOR_ ## COL + BACK_OFFSET)
#define FORE(COL) (COLOR_ ## COL + FORE_OFFSET)

#endif /* COLORS_H_ */