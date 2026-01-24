#include "../lib/includes/lib.h"

int y = 100;
int main() {
  signed char u = y == 100 ? -10 : 20;
  char str[256];
  itoa(u, str, 10);
  puts(str);
  return u;
}
