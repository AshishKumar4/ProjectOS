#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H

#include "types.h"

void itoa(int, char *, char);
void reverse (char *);

size_t strlen(char *s);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);
char *strcat(char *, const char *);
char *strncat(char *, const char*, size_t);
char *strcpy(char *, const char *);
void *memcpy(void *, const void *, size_t);
void *memset(void *, int c, size_t n);
#endif
