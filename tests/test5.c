#include "../lib/includes/syscall.h"

int main() {
  char *msg = "Hello, World";
  write(1, msg, 12); /* length is without the nul terminator */
  return 0;
}

