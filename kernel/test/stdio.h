#ifndef INCLUDE_STDIO_H 
#define INCLUDE_STDIO_H

#include "stdarg.h"
#include "types.h"

#define MAXLEN 1024
#define WHITE_BLACK 0x07
#define RED_BLACK 0x04

// Prototypes 
int putc(unsigned char);
int printk(const char *, ...);

void clear_screen(void);
void debug(const char *str, ...);
void vsprintf(char *, const char *, va_list);

#endif
