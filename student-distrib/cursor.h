/* cursor.h, put the functions related to the cursor inside this file, written by Bill */


#ifndef CURSOR_H
#define CURSOR_H


#include "keyboard.h"
#include "types.h"

/* used to update cursor's positon based on video memory */
void update_cursor(int pos_x, int pos_y);


int32_t cur_cursor_x;
int32_t cur_cursor_y;


#endif
