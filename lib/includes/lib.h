static void itoa(int value, char *str, int base) {
  static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int i = 0;
  int neg = 0;

  // Handle negative numbers
  if (value < 0) {
    neg = 1;
    value = -value;
  }

  // Convert digits
  do {
    str[i++] = digits[value % base];
    value /= base;
  } while (value > 0);

  // Add minus sign if negative
  if (neg) {
    str[i++] = '-';
  }

  // Null-terminate
  str[i] = '\0';

  // Reverse the string
  int left = 0;
  int right = i - 1;
  while (left < right) {
    char temp = str[left];
    str[left] = str[right];
    str[right] = temp;
    left++;
    right--;
  }
}
