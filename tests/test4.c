#include "../lib/includes/lib.h"

int factorial(int n) {
  if (n > 1) {
    return n * factorial(n - 1);
  } else
    return 1;
}

int main() {
  volatile int res = factorial(5);
  char str[256] = {0};
  itoa(res, str, 10);
  fputs("The result is: ", stdout);
  puts(str);
  return 0;
}
