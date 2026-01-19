
int f(int n) {
  if (n > 1)
    return n + f(n - 1);
  else
    return 1;
}

int main() { return f(5); }
