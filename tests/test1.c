#include "../lib/includes/lib.h"

static int z = 100;

int main() {
  int a = 101;
  int b = 100 + z;
  char str[256];
  itoa(a + b, str, 10);
  fputs((char *)"The result is: ", stdout);
  puts(str);
  return 0;
}
