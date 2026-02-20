#include <math.h>
#include <stdio.h>
#include <stdlib.h>


# define PLANCK_CST 1
# define MASS 1
# define N 10000
# define L 2

// Here instead of finding the orbits (Bohr)
// we will try to find energy eigenvalues (allowed energy levels)
// and eigenfunctions (waves functions)
// -h²/2m d²psi/dx² + V(x)psi = Epsi


// level | Calculated Energy | Analytical Theory | %error 
// ------------------------------------------------------

//extern void dstev_(char *jobz, int *n, double *d, double *e, 
                   double *z, int *ldz, double *work, int *info);

double potentiel(double x)
{
  if (x >= 0 && x <= (int)L/2)
    return 0;
  else 
    return 181818;
}
typedef struct s_sys
{
  double *x;
  double *diag;
  double *o;
  double *z;
  double *psi;
  double dx;
} t_sys;


int init_stuff(t_sys *sys)
{
  sys->x = malloc(sizeof(double) * N); // the grid
  sys->psi = malloc(sizeof(double) * N);// psi_i (the amplitude for each x[i])
  sys->diag = malloc(sizeof(double) * N);
  sys->z = malloc(sizeof(double) * N * N);
  sys->o = malloc(sizeof(double) * N);
  sys->dx = (N - 1)/L;
  return 0;
}
int main(void)
{
  t_sys *sys = malloc(sizeof(t_sys));
  double off_diag = -0.5 / (sys->dx*sys->dx);
  init_stuff(sys);
  // we put alpha = -h²/2m for simplification of the code 
  //double alpha = - (PLANCK_CST * PLANCK_CST)/ (2*MASS); // in our case is alpha = -1/2
  
  // init the gride
  // fixing the factor in diag 
  double k = 1.0 / (sys->dx *sys->dx);
  for (int i = 0; i < N; i++)
  {
    sys->x[i] = sys->dx * i;
    sys->diag[i] = k + potentiel(sys->x[i]);
    if (i < N -1)
      sys->o[i] = off_diag;
  }
  // we need to solve this matrix (see the readme for more mathematical context)

  //dstev_('V', N, sys->diag, sys->o, N,  )
  return 0;
}
