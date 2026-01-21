#include "../includes/lib.h"
#include "../includes/syscodes.h"

// Function to print a buffer using RISC-V ecall
void write(int fd, void *buf, unsigned int len) {
  register int a0 asm("a0") = fd;           /* file descriptor to write to */
  register void *a1 asm("a1") = buf;        /* address */
  register unsigned int a2 asm("a2") = len; /* length of the buffer */
  register long a7 asm("a7") = SYS_WRITE;   /* SYS_WRITE */

  asm volatile("ecall"
               : /* no output */
               : "r"(a0), "r"(a1), "r"(a2), "r"(a7));
}

void itoa(int value, char *str, unsigned int base) {
  static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i = 0;
  int neg = 0;

  // Handle negative numbers
  if (value < 0) {
    neg = 1;
    value = -value;
  }

  // Convert digits
  do {
    str[i++] = digits[value % base];
    value /= base;
  } while (value > 0);

  // Add minus sign if negative
  if (neg) {
    str[i++] = '-';
  }

  // Null-terminate
  str[i] = '\0';

  // Reverse the string
  int left = 0;
  int right = i - 1;
  while (left < right) {
    char temp = str[left];
    str[left] = str[right];
    str[right] = temp;
    left++;
    right--;
  }
}

inline unsigned int strlen(const char *str) {
  unsigned int ret = 0;
  char x;
  while ((x = str[ret]) != '\0')
    ++ret;
  return ret;
}

inline char *strcat(char *dest, const char *s) {
  unsigned int ret = strlen(dest);
  unsigned int ss = 0;
  while (s[ss] != '\0')
    dest[ret] = s[ss++];
  dest[ret + 1] = '\0';
  return dest;
}

inline void *memcpy(void *dest, const void *src, unsigned int bytes) {
  for (unsigned int ret = 0; ret < bytes; ++ret)
    ((char *)dest)[ret] = ((char *)src)[ret];
  return dest;
}

inline void *memset(void *ptr, int c, unsigned int n) {
  char *p = (char *)ptr;
  for (unsigned int i = 0; i < n; ++i) {
    p[i] = c;
  }
  return ptr;
}

void putc(volatile char c) { write(stdout, (void *)&c, 1); }

void puts(const char *s) {
  write(stdout, (void *)s, strlen(s));
  putc('\n');
}

void fputs(const char *s, unsigned int fd) { write(fd, (void *)s, strlen(s)); }
