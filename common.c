#include "common.h"
#include <math.h>

double absf(double f) {
  if (f < 0) {
    return -f;
  } else {
    return f;
  }
}

double norm_pdf(double x) { return exp(-x * x / 2) / sqrt(2 * M_PI); }

// Normal CDF approximation by Zelen & Severo
double norm_cdf(double x) {
  double t;

  if (x == 0) {
    return 0.5;
  } else if (x < 0) {
    return 1 - norm_cdf(-x);
  }

  t = 1 / (1 + 0.2316419 * x);
  return 1 - norm_pdf(x) * (0.319381530 * t - 0.356563782 * t * t +
                    1.781477937 * t * t * t - 1.821255978 * t * t * t * t +
                    1.330274429 * t * t * t * t * t);
}
