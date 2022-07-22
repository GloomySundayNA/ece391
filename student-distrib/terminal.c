

#include "terminal.h"


int terminal_enabled1;
int terminal_enabled2;
int terminal_enabled3;

#define MAXIMUM_LENGTH 128
/* int32_t terminal_open(const uint8_t* filename)
 * Inputs: filename -- pointer to the name of the file
 * Return Value: 0 on success
 * Function: open the terminal
 */
extern int32_t terminal_open(const uint8_t* filename){
    //initialize
    return 0;
}


/* int32_t terminal_close(int32_t fd)
 * Inputs: fd -- file discriptor
 * Return Value: 0 on success
 * Function: close the terminal
 */
extern int32_t terminal_close(int32_t fd){
    return -1;
}


/* int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: fd -- file discriptor
 *         buf -- buffer that stores the input string
 *         nbytes -- the number of bytes to be read
 * Return Value: number of bytes read
 * Function: read the input from the user input
 */
extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    if (buf == NULL) {
        return -1;
    }
    int i;
    if(cur_execute_term == 0){
        terminal_enabled1 = 1;
        //cast buffer from void to int
        read_flag1 = 0;
        sti();
        while (1)
        {
            if (read_flag1 == 1) {
                break;
            }
        }
        read_flag1 = 0;
        terminal_enabled1 = 0;
        //assign what's in key buffer to buffer
        for (i = 0; i < nbytes; i++)
        {
            if (i == MAXIMUM_LENGTH - 1 || line_buf1[i] == '\n'){
                *((uint8_t*)buf + i) = '\n';
                break;
            }
            //printf("%c", line_buf[i]);
            *((uint8_t*)buf + i) = line_buf1[i];
        }
        //printf("%s", ((uint8_t*)buf));
        //assign the last char for break
        // casted_buffer[i] = '\n';
        *((uint8_t*)buf+i+1) = '\0';

        //clear the keyboard buffer
        // clear_line_buffer();
        
        //casted_buffer[127] = '\n';

        
        return i + 1;
    }
    else if(cur_execute_term == 1){
        terminal_enabled2 = 1;
        //cast buffer from void to int
        read_flag2 = 0;
        sti();
        while (1)
        {
            if (read_flag2 == 1) {
                break;
            }
        }
        read_flag2 = 0;
        terminal_enabled2 = 0;
        //assign what's in key buffer to buffer
        for (i = 0; i < nbytes; i++)
        {
            if (i == MAXIMUM_LENGTH - 1 || line_buf2[i] == '\n'){
                *((uint8_t*)buf + i) = '\n';
                break;
            }
            //printf("%c", line_buf[i]);
            *((uint8_t*)buf + i) = line_buf2[i];
        }
        //printf("%s", ((uint8_t*)buf));
        //assign the last char for break
        // casted_buffer[i] = '\n';
        *((uint8_t*)buf+i+1) = '\0';

        //clear the keyboard buffer
        // clear_line_buffer();
        
        //casted_buffer[127] = '\n';

        
        return i + 1;
    }
    else{
        terminal_enabled3 = 1;
        //cast buffer from void to int
        read_flag3 = 0;
        sti();
        while (1)
        {
            if (read_flag3 == 1) {
                break;
            }
        }
        read_flag3 = 0;
        terminal_enabled3 = 0;
        //assign what's in key buffer to buffer
        for (i = 0; i < nbytes; i++)
        {
            if (i == MAXIMUM_LENGTH - 1 || line_buf3[i] == '\n'){
                *((uint8_t*)buf + i) = '\n';
                break;
            }
            //printf("%c", line_buf[i]);
            *((uint8_t*)buf + i) = line_buf3[i];
        }
        //printf("%s", ((uint8_t*)buf));
        //assign the last char for break
        // casted_buffer[i] = '\n';
        *((uint8_t*)buf+i+1) = '\0';
        return i + 1;
    }
    return -1;
}


/* int32_t terminal_write(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: fd -- file discriptor
 *         buf -- buffer that stores the input string
 *         nbytes -- the number of bytes to be write
 * Return Value: number of bytes written
 * Function: write the user input to terminal
 */
extern int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes){
    //int32_t i = 0;
    int i = 0;
    cli();
    if (buf == NULL) {
        return -1;
    }
    //cli();

    //output what's in buffer
    for (i = 0; i < nbytes; i++)
    {
        //puting something
        if(*((uint8_t*)buf + i) != 0){
            putct(*((uint8_t*)buf + i));
        }
            
    }
    sti();
    return i;

}





