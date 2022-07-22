
#ifndef _PIT_H
#define _PIT_H

#include "types.h"




volatile int32_t cur_execute_term;

//initialize pit
void PIT_init();
//handle pit interrupt
void PIT_handler();
//open 3 shell and scheduling
void scheduler();


#endif

