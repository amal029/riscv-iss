int f(int);
/* The first function should always be _start */
int _start() { return f(5); }

int f(int n) {
  if (n > 1)
    return n + f(n - 1);
  else
    return 1;
}
