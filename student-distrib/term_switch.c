
#include "term_switch.h"
#include "lib.h"
#include "pit.h"
#include "cursor.h"
#include "keyboard.h"


#define VID_MAP_START 0xB8000
#define FOUR_KB 0x1000
int cur_vis_term = 0;


/* term_switch
 * DESCRIPTION: called when alt+fn is pressed, should switch to display the next terminal
 * INPUTS: next_term -- integer indicating which terminal to switch to
 * OUTPUTS: none 
 * SIDEEFFECTS: switch which terminal to be displayed
 */
void term_switch(int32_t next_term){
    if (next_term == cur_vis_term) {
        return;         // if want to switch to the current terminal (stay in the same terminal), do nothing
    }
    // printf("switch to terminal %d", next_term);
    // should set page 
    /* store B8000 to cur_vis_term's background buffer */
    // 
    switch_term_vim(cur_vis_term, cur_vis_term);
    memcpy((void*)(VID_MAP_START+ (cur_vis_term+1)*FOUR_KB), (void*)VID_MAP_START, FOUR_KB);        // +1 because terminal's id is 0 indexed
    /* store next_term's background buffer to B8000 */

    memcpy((void*)VID_MAP_START, (void*)(VID_MAP_START+ (next_term+1)*FOUR_KB), FOUR_KB);
    
    //int i;
    /* store old cursor and key_buf to term struct*/
    if (cur_vis_term == 0) {
        screen_x1 = screen_x;
        screen_y1 = screen_y;
        // for (i = 0; i < 128; i++) {
        //     term1.term_key_buf[i] = key_buf[i];
        // }
        //term1.term_key_idx = curr_idx;
        term1.term_CAPS_state = CAPS_state;
    } else if (cur_vis_term == 1) {
        screen_x2 = screen_x;
        screen_y2 = screen_y;
        // for (i = 0; i < 128; i++) {
        //     term2.term_key_buf[i] = key_buf[i];
        // }
        //term2.term_key_idx = curr_idx;
        term2.term_CAPS_state = CAPS_state;
    } else {
        screen_x3 = screen_x;
        screen_y3 = screen_y;
        // for (i = 0; i < 128; i++) {
        //     term3.term_key_buf[i] = key_buf[i];
        // }
        //term3.term_key_idx = curr_idx;
        term3.term_CAPS_state = CAPS_state;
    }


    /* update cursor on screen; update keyboard buffer*/
    if (next_term == 0) {
        //update_cursor(term1.cursor_x, term1.cursor_y);
        screen_x = screen_x1;
        screen_y = screen_y1;
        update_cursor(screen_x, screen_y);
        // for (i = 0; i < 128; i++) {
        //     key_buf[i] = term1.term_key_buf[i];
        // }
        //curr_idx = term1.term_key_idx;
        CAPS_state = term1.term_CAPS_state;
        //update current term
        cur_vis_term = next_term;
    } else if (next_term == 1) {
        //update_cursor(term2.cursor_x, term2.cursor_y);
        screen_x = screen_x2;
        screen_y = screen_y2;
        update_cursor(screen_x, screen_y);
        // for (i = 0; i < 128; i++) {
        //     key_buf[i] = term2.term_key_buf[i];
        // }
        //curr_idx = term2.term_key_idx;
        CAPS_state = term2.term_CAPS_state;

        cur_vis_term = next_term;
    } else {
        //update_cursor(term3.cursor_x, term3.cursor_y);
        screen_x = screen_x3;
        screen_y = screen_y3;
        update_cursor(screen_x, screen_y);
        // for (i = 0; i < 128; i++) {
        //     key_buf[i] = term3.term_key_buf[i];
        // }
        //curr_idx = term3.term_key_idx;
        CAPS_state = term3.term_CAPS_state;

        cur_vis_term = next_term;
    }
    
    return;
}

/*
physical 
b8000  -> video memo = screen   
bb1   -> 1
bb2   -> 2
bb3


virtual
b8000  --> video memo


alt + f1 --> detect this sequence, call terminal switch with input = 1 term_switch(0)
atl+f2  --> term_Switch(1)
*/
