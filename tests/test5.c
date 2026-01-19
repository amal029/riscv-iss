#include "../lib/includes/start.h"
#include "../lib/includes/syscall.h"
#include "../lib/includes/syscodes.h"

int main() {
  char *msg = "Hello, World";
  write(1, msg, 12); /* length is without the nul terminator */
  return 0;
}

