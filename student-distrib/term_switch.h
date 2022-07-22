
#ifndef _TERM_SWITCH_H
#define _TERM_SWITCH_H

#include "types.h"
#include "systemcall_handler.h"


// current executing pid for each terminal   [0,1,2]  -> [0,3,4] (shell, fish, pingpong)
// 3 here represents the number of terminals we have
int32_t term_pid_array[3];
// current terminal that's being displayed
int cur_vis_term;

typedef struct term_t {
    //int8_t term_key_buf[128];   //key_buf
    int32_t term_key_idx;       //curr_idx
    // a pointer should be enough 
    struct pcb* curr_execute_pid_pcb;
    int32_t cursor_x;
    int32_t cursor_y;
    int32_t term_CAPS_state;
    int32_t term_ebp;
    int32_t term_esp;
} term_t;

term_t term1;
term_t term2;
term_t term3;

//this is for switching to next terminal
void term_switch(int32_t next_term);

#endif

