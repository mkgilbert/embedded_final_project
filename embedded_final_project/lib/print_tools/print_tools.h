/*
 * print_tools.h
 *
 * Created: 12/1/2015 3:47:29 PM
 *  Author: Harrison
 */


 #ifndef PRINT_TOOLS_H_
 #define PRINT_TOOLS_H_

 void pt_clear_screen();
 void pt_set_color_modifier(unsigned char mod);
 void pt_clear_colors();
 void pt_set_cursor_pos(unsigned char x, unsigned char y);
 void pt_move_cursor_x(char offset);
 void pt_move_cursor_y(char offset);
 void pt_save_cursor_pos();
 void pt_restore_cursor_pos();
unsigned char pt_get_opposite_color(unsigned char color);

#endif /* PRINT_TOOLS_H_ */