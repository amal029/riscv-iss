#pragma once

#define stdout 1
#define stderr 2
#define stdin 0
#define NULL ((void*)0)

/* This library only supports 32-bit RISCV architecture */

/* We have a single lib with all the library function defined */

void write(int fd, void *buf, unsigned int len);

/* Function to convert int to string, with \0 termination */
void itoa(int, char *, unsigned int);

/* Function to convert float to string with \0 termination */
void ftoa(float num, char *str, int str_bytes);

/* Function to get the length of the string without \0 terminator */
unsigned int strlen(const char *);

/* Function to concat two strings; does not change the input string.
   Terminates the string with NUL*/
char *strcat(char *dest, const char *s2);

/* Copy from source to destination */
void *memcpy(void *dest, const void *src, unsigned int bytes);

/* Set memory */
void *memset(void *ptr, int value, unsigned int bytes);

/* Output a string */
void puts(const char *s);

/* Output a character */
void putc(char c);

/* Write to provided file descriptor */
void fputs(const char *, unsigned int);

/* Compare two strings. Does not check for NULL */
int strcmp(const char *s1, const char *s2);

/* Compare bytes arrays as unsigned char */
int memcmp(const void *s1, const void *s2, unsigned int bytes);
