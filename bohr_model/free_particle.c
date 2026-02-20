#include <math.h>
#include <stdio.h>
#include <stdlib.h>


# define R 1
# define MASSE 1 
# define PLANCK_CST 1 
# define N 1000
# define TIME 20 // sec 
# define DT 0.1 
# define QUANT_N  3

int main(void)
{
  FILE *f = fopen("wavefct.c", "w");
  if (!f)
    return -1;
  fprintf(f,"t,x,real_psi, image_psi, proba_density\n");

  double x[N];
  double real_psi = 0;
  double image_psi = 0;
  double prob_dens = 0;
  double t = 0;
  for (int i = 0; i < N ; i++)
  {
    x[i] = (2 * M_PI * i) / (double)N;
  }
  double energy = (double)(QUANT_N * QUANT_N)/ 2.0;
  int max_steps = (int)TIME/DT;
  for(int j = 0; j < max_steps; j++)
  {
    for (int i = 0; i <= N - 1; i++)
    {
      double theta = QUANT_N * x[i]  - energy * t;
      real_psi =  cos(theta);
      image_psi = sin(theta);
      prob_dens = real_psi * real_psi + image_psi * image_psi;
      fprintf(f, "%lf,%lf,%lf,%lf,%lf\n", t, x[i], real_psi, image_psi, prob_dens);
    }
    t += DT;
  }
  fclose(f);
  return 0;
}
