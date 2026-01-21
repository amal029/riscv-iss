#include "../includes/lib.h"

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
