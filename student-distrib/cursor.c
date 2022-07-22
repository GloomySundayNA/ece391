/* 
 * contributed by Bill, 
 * refers to http://www.osdever.net/FreeVGA/vga/crtcreg.htm#0B
 * and https://wiki.osdev.org/Text_Mode_Cursor
 */


#include "cursor.h"
#include "lib.h"


#define ADDR_REG  0x3D4
#define DATA_REG  0x3D5
#define ROW       80
#define REG_CLLR  0x0F
#define REG_CLHR  0x0E 
#define OFF_SET   0xFF
#define BIT_SHIFT 0x08
/*
 * update_cursor
 * DESCRIPTION: update the position of cursor, to be called by putc() in lib.c 
 * INPUITS: pos_x, pos_y
 * OUTPUTS: none
 * SIDEEFFECTS: update the position of cursor.
 */ 
void update_cursor(int pos_x, int pos_y){
    // position x and y to be determined by the writting video memory
	uint16_t pos = pos_y * ROW + pos_x;
	cur_cursor_x = pos_x;
	cur_cursor_y = pos_y;
    
	outb(REG_CLLR, ADDR_REG);
	outb((uint8_t) (pos & OFF_SET), DATA_REG);
	outb(REG_CLHR, ADDR_REG);
	outb((uint8_t) ((pos >> BIT_SHIFT) & OFF_SET), DATA_REG);
}


