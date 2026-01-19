int factorial(int n);

void _start() {
  factorial(5);
  /* Just loop here at the end */
  while(1);
}

int factorial(int n) {
  if (n > 1) {
    return n * factorial(n - 1);
  } else
    return 1;
}
