#include "common.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char parameter[1024], buf[16];
  int popsize, n;
  double p0;
  char tail;
  double p_hat, z;
  double p;
  double alpha;
  FILE *report;

  if (argc != 1) {
    fprintf(stderr, "usage: %s\n", argv[0]);
    return 1;
  }

  report = fopen("out-ztest.latex", "w");
  if (report == NULL) {
    perror("out-ztest.latex");
    return 1;
  }
  fprintf(report, "\\documentclass{article}\n");
  fprintf(report, "\\begin{document}\n");

  // Parameter
  fprintf(report, "\\begin{section}{Parameter of Interest}\n");
  printf("Let p be ");
  fflush(stdout);
  fgets(parameter, 1023, stdin);
  parameter[strlen(parameter) - 1] = '\0';
  fprintf(report, "Let $p$ be %s.\n", parameter);

  printf("The population size is (omit for sufficiently large "
         "populations) ");
  fflush(stdout);
  fgets(buf, 16, stdin);
  if (buf[0] == '\n') {
    popsize = -1;
    fprintf(report, "The population size is sufficiently "
                    "large.\n\n");
  } else {
    popsize = atoi(buf);
    fprintf(report, "The population size is %d.\n\n", popsize);
  }
  fprintf(report, "\\end{section}");

  // Hypotheses
  fprintf(report, "\\begin{section}{Hypotheses}\n");
  printf("H0: p = ");
  fflush(stdout);
  fgets(buf, 15, stdin);
  p0 = atof(buf);
  fprintf(report, "$H_0: p = %g$", p0);

  printf("HA: p ? %g (ne/gt/lt) ", p0);
  fflush(stdout);
  fgets(buf, 15, stdin);
  tail = buf[0];
  if (tail == 'n') {
    fprintf(report, "\\qquad $H_A: p \\ne %g$\n\n", p0);
  } else if (tail == 'g') {
    fprintf(report, "\\qquad $H_A: p > %g$\n\n", p0);
  } else if (tail == 'l') {
    fprintf(report, "\\qquad $H_A: p < %g$\n\n", p0);
  } else {
    fprintf(stderr, "invalid input (expected 'ne', 'gt', or "
                    "'lt')\n");
    return 1;
  }
  fprintf(report, "\\end{section}\n");

  // Assumptions
  fprintf(report, "\\begin{section}{Verifying Assumptions}\n");
  printf("n=");
  fflush(stdout);
  fgets(buf, 15, stdin);
  n = atoi(buf);
  fprintf(stderr, "$n = %d$\n\n", n);

  printf("SRS, independent, ");
  fprintf(report, "SRS, independent, ");
  if (n * p0 > 10) {
    printf("n > 10");
    fprintf(report, "$n > 10$");
  } else {
    printf("n <= 10 (proceed with caution)");
    fprintf(report, "$n \\le 10$ (proceed with caution)");
  }
  if (popsize == -1 || n * 10 <= popsize) {
    printf(", n <= 10%% of population size\n");
    fprintf(report, ", $n \\le 10\\%%$ of population size\n");
  } else {
    printf(", n > 10%% of population size (proceed with "
           "caution\n");
    fprintf(report, ", $n > 10\\%%$ of population size (proceed "
                    "with caution\n");
  }
  fprintf(report, "\\end{section}\n");

  // Name
  fprintf(report, "\\begin{section}{Name of Test}\n");
  printf("One proportion z-test\n");
  fprintf(report, "One proportion z-test\n\n");
  fprintf(report, "\\end{section}\n");

  // Test statistic
  fprintf(report, "\\begin{section}{Test Statistic}\n");
  printf("p_hat = ");
  fflush(stdout);
  fgets(buf, 15, stdin);
  p_hat = atof(buf);
  fprintf(report, "$\\hat p = %g$\n\n", p_hat);

  z = (p_hat - p0) / sqrt(p0 * (1 - p0) / n);
  printf("z = (p_hat-p0)/sqrt(p(1-p)/n) = (%g-%g)/sqrt(%g*(1-%g)/%d) = %g\n",
         p_hat, p0, p0, p0, n, z);
  fprintf(report,
          "$\\hat z = "
          "\\frac{\\hat p - p_0}{\\sqrt{p(1-p) / n}} = "
          "\\frac{%g-%g}{\\sqrt{%g(1-%g) / %d}} = "
          "%g$\n\n",
          p_hat, p0, p0, p0, n, z);
  fprintf(report, "\\end{section}\n");

  // P value
  fprintf(report, "\\begin{section}{$p$-value}\n");
  if (tail == 'n') {
    double absz;

    absz = absf(z);
    p = 2 * norm_cdf(-absz);
    printf("P(z < -%g or z > %g) = 2*P(z > %g) = %g\n", absz, absz, absz, p);
    fprintf(report,
            "$P(z < -|\\hat z| \\cup z > |\\hat z|) = "
            "2*P(z > |\\hat z|) = "
            "2*P(z > %g) = %g$\n\n",
            absz, p);
  } else if (tail == 'l') {
    p = norm_cdf(z);
    printf("P(z < %g) = %g\n", z, p);
    fprintf(report, "$P(z < \\hat z) = P(z < %g) = %g$\n\n", z, p);
  } else {
    p = 1 - norm_cdf(z);
    printf("P(z > %g) = %g\n", z, p);
    fprintf(report, "$P(z > \\hat z) = P(z > %g) = %g$\n\n", z, p);
  }
  fprintf(report, "\\end{section}\n");

  // Conclusion
  fprintf(report, "\\begin{section}{Conclusion}\n");
  printf("alpha = ");
  fflush(stdout);
  fgets(buf, 15, stdin);
  alpha = atof(buf);
  fprintf(report, "$\\alpha = %g$\n\n", alpha);

  if (p < alpha) {
    char *comparison;

    if (tail == 'n')
      comparison = "not";
    else if (tail == 'l')
      comparison = "less than";
    else
      comparison = "more than";
    printf("Reject H0. There is significant evidence (p=%g) that "
           "%s is %s %g.\n",
           p, parameter, comparison, p0);
    fprintf(report,
            "Reject $H_0$. There is significant evidence "
            "(p=%g) that %s is %s %g.\n\n",
            p, parameter, comparison, p0);
  } else {
    printf("Fail to reject H0. There is not enough evidence "
           "($p=%g$) make a conclusion.\n",
           p);
    fprintf(report,
            "Fail to reject $H_0$. There is not enough "
            "evidence ($p=%g$) make a conclusion.\n",
            p);
  }
  fprintf(report, "\\end{section}");

  fprintf(report, "\\end{document}");
  fclose(report);
  return 0;
}
