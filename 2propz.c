#include "common.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char param1[1024], param2[1024], buf[16];
  int popsize1, popsize2, x1, x2, n1, n2;
  double p0;
  char tail;
  double pooled_p_hat, p1_hat, p2_hat, z;
  double p;
  double alpha;
  FILE *report;

  if (argc != 1) {
    fprintf(stderr, "usage: %s\n", argv[0]);
    return 1;
  }

  report = fopen("out-2propz.latex", "w");
  if (report == NULL) {
    perror("out-ztest.latex");
    return 1;
  }
  fprintf(report, "\\documentclass{article}\n");
  fprintf(report, "\\setlength\\parindent{0pt}");
  fprintf(report, "\\begin{document}\n");

  // Parameter
  fprintf(report, "\\begin{section}{Parameter of Interest}\n");
  printf("Let p1 be ");
  fflush(stdout);
  fgets(param1, 1023, stdin);
  param1[strlen(param1) - 1] = '\0';
  fprintf(report, "Let $p_1$ be %s.\n", param1);

  printf("Let p2 be ");
  fflush(stdout);
  fgets(param2, 1023, stdin);
  param2[strlen(param2) - 1] = '\0';
  fprintf(report, "Let $p_2$ be %s.\n", param2);

  printf("The first population size is (omit for sufficiently large "
         "populations) ");
  fflush(stdout);
  fgets(buf, 16, stdin);
  if (buf[0] == '\n') {
    popsize1 = -1;
    fprintf(report, "The first population size is sufficiently large.\n\n");
  } else {
    popsize1 = atoi(buf);
    fprintf(report, "The first population size is %d.\n", popsize1);
  }

  printf("The second population size is (omit for sufficiently large "
         "populations) ");
  fflush(stdout);
  fgets(buf, 16, stdin);
  if (buf[0] == '\n') {
    popsize2 = -1;
    fprintf(report, "The second population size is sufficiently large.\n\n");
  } else {
    popsize2 = atoi(buf);
    fprintf(report, "The second population size is %d.\n\n", popsize2);
  }
  fprintf(report, "\\end{section}");

  // Hypotheses
  fprintf(report, "\\begin{section}{Hypotheses}\n");
  printf("H0: p1 - p2 = ");
  fflush(stdout);
  fgets(buf, 15, stdin);
  p0 = atof(buf);
  fprintf(report, "$H_0: p_1 - p_2 = %g$", p0);

  printf("HA: p1 - p2 ? %g (ne/gt/lt) ", p0);
  fflush(stdout);
  fgets(buf, 15, stdin);
  tail = buf[0];
  if (tail == 'n') {
    fprintf(report, "\\qquad $H_A: p_1 - p_2 \\ne %g$\n\n", p0);
  } else if (tail == 'g') {
    fprintf(report, "\\qquad $H_A: p_1 - p_2 > %g$\n\n", p0);
  } else if (tail == 'l') {
    fprintf(report, "\\qquad $H_A: p_1 - p_2 < %g$\n\n", p0);
  } else {
    fprintf(stderr, "invalid input (expected 'ne', 'gt', or "
                    "'lt')\n");
    return 1;
  }
  fprintf(report, "\\end{section}\n");

  // Assumptions
  fprintf(report, "\\begin{section}{Verifying Assumptions}\n");
  printf("n1=");
  fflush(stdout);
  fgets(buf, 15, stdin);
  n1 = atoi(buf);
  fprintf(report, "$n_1 = %d$\n", n1);

  printf("x1=");
  fflush(stdout);
  fgets(buf, 15, stdin);
  x1 = atoi(buf);
  fprintf(report, "\\qquad $x_1 = %d$\n", x1);
  p1_hat = (double)x1 / n1;
  printf("p1_hat = x1/n = %g\n", p1_hat);
  fprintf(report, "\\qquad $\\hat p_1 = x_1 / n = %d / %d = %g$\n\n", x1,
          n1, p1_hat);

  printf("n2=");
  fflush(stdout);
  fgets(buf, 15, stdin);
  n2 = atoi(buf);
  fprintf(report, "$n_2 = %d$\n", n2);

  printf("x2=");
  fflush(stdout);
  fgets(buf, 15, stdin);
  x2 = atoi(buf);
  fprintf(report, "$\\qquad x_2 = %d$\n", x2);
  p2_hat = (double)x2 / n2;
  printf("p1_hat = x2/n = %g\n", p2_hat);
  fprintf(report, "\\qquad $\\hat p_2 = x_2 / n = %d / %d = %g$\n\n", x2,
          n2, p2_hat);

  printf("SRS, independent, ");
  fprintf(report, "SRS, independent, ");
  if (n1 * p1_hat > 10) {
    printf("n1*p1_hat > 10");
    fprintf(report, "$n_1 \\hat p_1 > 10$");
  } else {
    printf("n1*p1_hat <= 10 (proceed with caution)");
    fprintf(report, "$n_1 \\hat p_1 \\le 10$ (proceed with caution)");
  }
  if (popsize1 == -1 || n1 * 10 <= popsize1) {
    printf(", n1 <= 10%% of the first population size\n");
    fprintf(report, ", $n_1 \\le 10\\%%$ of the first population size\n");
  } else {
    printf(", n1 > 10%% of the first population size (proceed with caution\n");
    fprintf(report, ", $n_1 > 10\\%%$ of the first population size (proceed "
                    "with caution\n");
  }
  if (n2 * p2_hat > 10) {
    printf("n2*p2_hat > 10");
    fprintf(report, "$n_2 \\hat p_2 > 10$");
  } else {
    printf("n2*p2_hat <= 10 (proceed with caution)");
    fprintf(report, "$n_2 \\gat p_2 \\le 10$ (proceed with caution)");
  }
  if (popsize2 == -1 || n2 * 10 <= popsize2) {
    printf(", n2 <= 10%% of the second population size\n");
    fprintf(report, ", $n_2 \\le 10\\%%$ of the second population size\n");
  } else {
    printf(", n2 > 10%% of the second population size (proceed with caution\n");
    fprintf(report, ", $n_2 > 10\\%%$ of the second population size (proceed "
                    "with caution\n");
  }
  fprintf(report, "\\end{section}\n");

  // Name
  fprintf(report, "\\begin{section}{Name of Test}\n");
  printf("Two proportion z-test\n");
  fprintf(report, "Two proportion z-test\n\n");
  fprintf(report, "\\end{section}\n");

  // Test statistic
  fprintf(report, "\\begin{section}{Test Statistic}\n");
  pooled_p_hat = (p1_hat + p2_hat) / (n1 + n2);
  printf("pooled p_hat  = (p1_hat + p2_hat) / (n1 + n2) = %g\n", pooled_p_hat);
  fprintf(report,
          "$\\hat p_{pooled} = \\frac{\\hat p_1 + \\hat p_2}"
          "{n_1 + n_2} = \\frac{%g + %g}{%d + %d} = %g$\n\n",
          p1_hat, p2_hat, n1, n2, pooled_p_hat);

  z = ((p1_hat - p2_hat) - p0) /
      sqrt(pooled_p_hat * (1 - pooled_p_hat) * (1.0 / n1 + 1.0 / n2));
  printf("z = ((p1_hat - p2_hat) - p0) / sqrt(pooled_p_hat * (1 - "
         "pooled_p_hat) * (1/n1 + 1/n2)) = ((%g - %g) - %g) / sqrt(%g * (1 - "
         "%g) * (1/%d + 1/%d))  = %g",
         p1_hat, p2_hat, p0, pooled_p_hat, pooled_p_hat, n1, n2, z);
  fprintf(report,
          "$\\hat z = \\frac{(\\hat p_1 - \\hat p_2) - p_0}{\\sqrt{\\hat "
          "p_{pooled}(1 - "
          "\\hat p_{pooled})\\left(\\frac{1}{n_1} + \\frac{1}{n_2}\\right)}} = "
          "\\frac{(%g - %g) - %g}{\\sqrt{%g(1 - %g)\\left(\\frac{1}{%d} + "
          "\\frac{1}{%d}\\right)}} = %g$\n\n",
          p1_hat, p2_hat, p0, pooled_p_hat, pooled_p_hat, n1, n2, z);
  fprintf(report, "\\end{section}\n");

  // P value
  fprintf(report, "\\begin{section}{$p$-value}\n");
  if (tail == 'n') {
    double absz;

    absz = absf(z);
    p = 2 * norm_cdf(-absz);
    printf("P(z < -%g or z > %g) = 2*P(z > %g) = %g\n", absz, absz, absz, p);
    fprintf(report,
            "$P(z < -|\\hat z| \\cup z > |\\hat z|) = 2*P(z > |\\hat z|) = "
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
    printf("Reject H0. There is significant evidence (p=%g) that the "
           "difference between %s and %s is %s %g.\n",
           p, param1, param2, comparison, p0);
    fprintf(report,
            "Reject $H_0$. There is significant evidence ($p=%g$) that the "
            "difference between %s and %s is %s %g.\n\n",
            p, param1, param2, comparison, p0);
  } else {
    printf("Fail to reject H0. There is not enough evidence (p=%g) make a "
           "conclusion.\n",
           p);
    fprintf(report,
            "Fail to reject $H_0$. There is not enough evidence ($p=%g$) make "
            "a conclusion.\n",
            p);
  }
  fprintf(report, "\\end{section}");

  fprintf(report, "\\end{document}");
  fclose(report);
  return 0;
}
