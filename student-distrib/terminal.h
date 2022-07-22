
#ifndef TERMINAL_H
#define TERMINAL_H


#include "lib.h"
#include "types.h"
#include "keyboard.h"
#include "x86_desc.h"
#include "paging.h"
/* to open the terminal*/
extern int32_t terminal_open(const uint8_t* filename);
/* to close the terminal*/
extern int32_t terminal_close(int32_t fd);
/* to read from the terminal*/
extern int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
/* to write to the terminal*/
extern int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);


extern int terminal_enabled1;
extern int terminal_enabled2;
extern int terminal_enabled3;

#endif

