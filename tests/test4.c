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
  char str[5] = {0};
  itoa(res, str, 10);
  strcat(str, "\n");
  write(1, str, strlen(str));
  return 0;
}
