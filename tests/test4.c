#include "../lib/includes/lib.h"
#include "../lib/includes/syscall.h"

int factorial(int n) {
  if (n > 1) {
    return n * factorial(n - 1);
  } else
    return 1;
}

int main() {
  volatile int res = factorial(5);
  char str[4]; /* 4 numbers, + NUL */
  itoa(res, str, 10);
  write(1, str, 3);
  return 0;
}
