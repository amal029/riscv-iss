#include "../lib/includes/lib.h"

#define PI 3.141592653589f

// Function to compute sin(x) using Taylor series (x in radians)
float sin_taylor(float x) {
  // Reduce x to the range [-π, π] using periodicity
  x = x - (int)(x / (2 * PI)) * (2 * PI); // Normalize to [-2π, 2π]

  // Use symmetry: sin(-x) = -sin(x)
  int sign = (x < 0) ? 1 : -1;
  x = (x < 0) ? -x : x;

  // Use the iterative method to compute series efficiently
  float term = x;
  float sum = term;
  int n = 1; // Term index (1 for x^1/1!)

  // Continue until term becomes negligible
  while (term > 1e-6f || term < -1e-6f) {
    // Update term: next term = previous term * (-x²) / ((2n)(2n+1))
    term = term * (-x * x) / ((2 * n) * (2 * n + 1));
    sum += term;
    n++;
  }

  return sign * sum;
}

int main() {
  float angle_deg = -30.0f;
  float angle_rad = angle_deg * PI / 180.0f;
  float result = sin_taylor(angle_rad);
  char res[256];
  ftoa(result, res, 256);
  fputs("Sin -30 degrees is: ", stdout);
  puts(res);

  angle_deg = 30.0f;
  angle_rad = angle_deg * PI / 180.0f;
  result = sin_taylor(angle_rad);
  ftoa(result, res, 256);
  fputs("Sin 30 degrees is: ", stdout);
  puts(res);
  
  return 0;
}
