#pragma once

/* This library only supports 32-bit RISCV architecture */

/* We have a single lib with all the library function defined */

/* Function to convert int to string, with \0 termination */
void itoa(int, char *, unsigned int);

/* Function to get the length of the string without \0 terminator */
unsigned int strlen(const char *);

/* Function to concat two strings; does not change the input string.
   Terminates the string with NUL*/
char *strcat(char *dest, const char *s2);

/* Copy from source to destination */
void *memcpy(void *dest, const void *src, unsigned int bytes);
