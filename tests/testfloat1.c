#include "../lib/includes/lib.h"

int main(void) {
  float a = 10.4f;
  float b = 3.5f;
  float fres = a + b;
  char res[256];

  /* Float addition */
  ftoa(fres, res, 256);
  fputs("The result of float addition is: ", stdout);
  puts(res);

  /* Float multiplication */
  ftoa(a * b, res, 256);
  fputs("The result of float multiplication is: ", stdout);
  puts(res);

  /* Float subtraction */
  ftoa(b - a, res, 256);
  fputs("The result of float subtraction is: ", stdout);
  puts(res);

  /* Float division */
  ftoa(a / b, res, 256);
  fputs("The result of float division is: ", stdout);
  puts(res);

  /* Float fused mul-add -- not generated */
  float t = 7.8;
  ftoa(a * b + t, res, 256);
  fputs("The result of float fmadd is: ", stdout);
  puts(res);

  return 0;
}
