#include "../lib/includes/syscall.h"

int factorial(int n) {
  if (n > 1) {
    return n * factorial(n - 1);
  } else
    return 1;
}

int main() {
  int res = factorial(5);
  write(1, &res, 4);
}
