

#include "pit.h"
#include "lib.h"
#include "i8259.h"
#include "paging.h"
#include "systemcall_handler.h"
#include "term_switch.h"


#define PIT_IRQ_NUM     0x0
#define FERQ            11930

#define FOUR                4
#define EIGHT_KILOBYTES     8192
#define EIGHT_MEGABYTES     0x800000

#define MODE_THREE      0x36
#define MODE_PORT       0x43

#define SIXTY_FOUR      0x40
#define EIGHT           8

static int count = 0;
int32_t term1_boot = -1;
int32_t term2_boot = -1;
int32_t term3_boot = -1;


/* PIT_init
 * DESCRIPTION: Initialize the PIT interrupt by setting the Control register
 * and enable IRQ0
 * INPUTS:none
 * OUTPUTS:none
 * SIDEEFFECTS: enable the PIT interrupt
 */
void PIT_init() {
    // 0x43: set mode port , 0x36: set mode 3
    outb(MODE_THREE, MODE_PORT);
    // 1.193182 MHz / 11930 = 100 Hz
    // set the reload value 
    // low byte
    outb(FERQ & 0xFF, SIXTY_FOUR);
    // high byte
    outb((FERQ & 0xFF00)>>8, SIXTY_FOUR);

    enable_irq(PIT_IRQ_NUM);
}


/* PIT_handler
 * DESCRIPTION: the handler function of PIT interrupt, should be called
 * when PIT interrupt raised
 * INPUTS: none
 * OUTPUTS: send EOI signal 
 * SIDEEFFECTS: none
 */
void PIT_handler() {
    send_eoi(PIT_IRQ_NUM);
    // set the reload value 
    // low byte
    // printf("called %d \n", cur_execute_term);
    scheduler();
    //channel 0 data port
    outb(FERQ & 0xFF, SIXTY_FOUR);
    // high byte
    outb((FERQ & 0xFF00)>>8, SIXTY_FOUR);

}

/* scheduler
 * DESCRIPTION: Boot up 3 terminals, then perform generic shceduling which changes the video memo maping,
                TSS, and EBP ESP, to fake a multiprocessor
 * INPUTS: none
 * OUTPUTS: none 
 * SIDEEFFECTS: enable scheduling
 */
void scheduler() {
    // boot up 3 terminals

    int to_pid;
    if (term1_boot == -1) {
        term1_boot = 0;
        term_pid_array[0] = term1_boot;
        cur_execute_term = 0;
        // finish initializing term1 struct, set cursor_x, cursor_y etc to default value
        reset_screen();
        init_shell();
        return;
    }
    if (term2_boot == -1) {
        screen_x1 = screen_x;
        screen_y1 = screen_y;
        term1.term_CAPS_state = 0;
        term1.curr_execute_pid_pcb = current_pcb_ptr;
        //term1.term_key_idx = 0;
        // save term1's ebp and esp
        asm("                   \n\
        movl %%ebp, %0          \n\
        movl %%esp, %1          \n\
        "
        :"=r"(term1.term_ebp), "=r"(term1.term_esp)
        :/* no inputs */
        :"cc"
        );
        term2_boot = 1;
        term_pid_array[1] = term2_boot;
        cur_execute_term = 1;
        // finish initializing term2 struct, set cursor_x, cursor_y etc to default value
        // printf("A breakpoint.\n");
        switch_term_vim(0, 1);
        // printf("A breakpoint.\n");
        // reset_screen();
        init_shell();
        return;
    }
    if (term3_boot == -1) {
        screen_x2 = screen_x;
        screen_y2 = screen_y;
        term2.term_CAPS_state = 0;
        //term2.term_key_idx = 0;
        term2.curr_execute_pid_pcb = current_pcb_ptr;
        
        asm("                   \n\
        movl %%ebp, %0          \n\
        movl %%esp, %1          \n\
        "
        :"=r"(term2.term_ebp), "=r"(term2.term_esp)
        :/* no inputs */
        :"cc"
        );

        //2 is the third terminal, zero index
        term3_boot = 2;
        term_pid_array[2] = term3_boot;
        cur_execute_term = 2;
        // finish initializing term3 struct, set cursor_x, cursor_y etc to default value
        switch_term_vim(0, 2);
        // reset_screen();
        init_shell();
        return;
    }
    // generic scheduling
    if(count == 0){
        count = 1;
        term3.curr_execute_pid_pcb = current_pcb_ptr;
        screen_x3 = screen_x;
        screen_y3 = screen_y;
        term3.term_CAPS_state = 0;
        //term3.term_key_idx = 0;

    }
    // context switch to the next terminal's EBP, ESP
    if (cur_execute_term == 0) {
        term1.curr_execute_pid_pcb = current_pcb_ptr;
        // current terminal 1 switch to terminal 2
        cur_execute_term = 1;
        
        term_pid_array[0] = current_id;
           
        to_pid = term_pid_array[1]; 
        current_id = to_pid;
        current_pcb_ptr = term2.curr_execute_pid_pcb;    // haven't initialize this yet
        
        /* step 1, switch video memo paging*/
        switch_term_vim(cur_vis_term, cur_execute_term);

        /* step 2, call set_page*/
        set_page(current_id);

        /* step 3, TSS.ESP0 */
        tss.esp0 = EIGHT_MEGABYTES - EIGHT_KILOBYTES*(current_id) -  FOUR;

        /* to sth with tss */
        // printf("reach this\n");
        // save current ebp and esp then load process to_pid's ebp and esp
        asm("                   \n\
        movl %%ebp, %0          \n\
        movl %%esp, %1          \n\
        "
        :"=r"(term1.term_ebp), "=r"(term1.term_esp)
        :
        :"cc"
        );
        
        asm("                   \n\
        movl %0, %%ebp          \n\
        movl %1, %%esp          \n\
        "
        :
        :"r"(term2.term_ebp), "r"(term2.term_esp)
        :"cc"
        );
        // printf("reach this\n");

    } else if (cur_execute_term == 1) {
        term2.curr_execute_pid_pcb = current_pcb_ptr;
        // current terminal 1 switch to terminal 2
        cur_execute_term = 2;
        
        //1 is the second terminal
        term_pid_array[1] = current_id;
          
        to_pid = term_pid_array[2]; 
        current_id = to_pid;
        current_pcb_ptr = term3.curr_execute_pid_pcb;    // haven't initialize this yet
    
        /* step 1, switch video memo paging*/
        switch_term_vim(cur_vis_term, cur_execute_term);

        /* step 2, call set_page*/
        set_page(current_id);

        /* step 3, TSS.ESP0 */
        tss.esp0 = EIGHT_MEGABYTES - EIGHT_KILOBYTES*(current_id) -  FOUR;

        /* to sth with tss */
        
        // save current ebp and esp then load process to_pid's ebp and esp
        asm("                   \n\
        movl %%ebp, %0          \n\
        movl %%esp, %1          \n\
        "
        :"=r"(term2.term_ebp), "=r"(term2.term_esp)
        :
        :"cc"
        );

        asm("                   \n\
        movl %0, %%ebp          \n\
        movl %1, %%esp          \n\
        "
        :
        :"r"(term3.term_ebp), "r"(term3.term_esp)
        :"cc"
        );

    } else if (cur_execute_term == 2) {
        term3.curr_execute_pid_pcb = current_pcb_ptr;
        // current terminal 2 switch to terminal 3
        cur_execute_term = 0;
        //2 is the third terminal
        term_pid_array[2] = current_id;
        // perform context switch   
        to_pid = term_pid_array[0]; 
        current_id = to_pid;
        current_pcb_ptr = term1.curr_execute_pid_pcb;    // haven't initialize this yet
        
        /* step 1, switch video memo paging*/
        switch_term_vim(cur_vis_term, cur_execute_term);

        /* step 2, call set_page*/
        set_page(current_id);
    
        /* step 3, TSS.ESP0 */
        tss.esp0 = EIGHT_MEGABYTES - EIGHT_KILOBYTES*(current_id) -  FOUR;


        /* to sth with tss */
        // printf("reach this\n");
        // save current ebp and esp then load process to_pid's ebp and esp
        asm("                   \n\
        movl %%ebp, %0          \n\
        movl %%esp, %1          \n\
        "
        :"=r"(term3.term_ebp), "=r"(term3.term_esp)
        :
        :"cc"
        );
        
        asm("                   \n\
        movl %0, %%ebp          \n\
        movl %1, %%esp          \n\
        "
        :
        :"r"(term1.term_ebp), "r"(term1.term_esp)
        :"cc"
        );
        
    }

}



