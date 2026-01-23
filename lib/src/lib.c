#include "../includes/lib.h"
#include "../includes/syscodes.h"

__attribute__((optimize("O1"))) void itoa(int value, char *str,
                                          unsigned int base) {
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

void ftoa(float num, char *str, int str_bytes) {
  char *p = str;

  // Handle negative numbers
  if (num < 0) {
    *p++ = '-';
    num = -num;
  }

  // Extract integer part
  int ipart = (int)num;
  itoa(ipart, p, 10);
  int str_len = strlen(p);
  p += str_len;

  // Add decimal point
  *p++ = '.';

  // Extract fractional part
  float fpart = num - ipart;
  int ndig = 0;
  char *endp = str + str_bytes - 1;

  /* The max number of digits after . is 3 */
  while (fpart != 0 && ndig < 3 && p < endp) {
    fpart *= 10;
    int digit = (int)fpart;
    *p++ = '0' + digit;
    fpart -= digit;
    ndig++;
  }

  *p = '\0';
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
    ((unsigned char *)dest)[ret] = ((unsigned char *)src)[ret];
  return dest;
}

inline void *memset(void *ptr, int c, unsigned int n) {
  char *p = (char *)ptr;
  for (unsigned int i = 0; i < n; ++i) {
    p[i] = c;
  }
  return ptr;
}

inline void putc(volatile char c) { write(stdout, (void *)&c, 1); }

inline void puts(const char *s) {
  write(stdout, (void *)s, strlen(s));
  putc('\n');
}

inline void fputs(const char *s, unsigned int fd) {
  write(fd, (void *)s, strlen(s));
}

inline int strcmp(const char *s1, const char *s2) {
  int ret;
  while ((ret = (*s1 - *s2)) == 0) {
    if (!*s1 || !*s2) {
      break;
    } else {
      ++s1;
      ++s2;
    }
  }
  return ret;
}

inline int memcmp(const void *s1, const void *s2, unsigned int bytes) {
  bytes -= 1;
  int ret;
  while ((ret = (*((unsigned char *)s1 + bytes) -
                 *((unsigned char *)s2 + bytes))) == 0) {
    --bytes;
  }
  return ret;
}
