#include "../lib/includes/lib.h"
#include "../lib/includes/syscall.h"

static int z = 100;

int main() {
  int a = 101;
  int b = 100 + z;
  char str[256];
  itoa(a + b, str, 10);
  strcat(str, "\n");
  write(1, str, strlen(str));
}
