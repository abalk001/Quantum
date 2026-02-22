#include <math.h>
#include <stdio.h>
#include <stdlib.h>


# define PLANCK_CST 1
# define MASS 1
# define N 1000
# define L 10

// Here instead of finding the orbits (Bohr)
// we will try to find energy eigenvalues (allowed energy levels)
// and eigenfunctions (waves functions)
// -h²/2m d²psi/dx² + V(x)psi = Epsi


// level | Calculated Energy | Analytical Theory | %error 
// ------------------------------------------------------

extern void dstev_(char *jobz, int *n, double *d, double *e, 
                   double *z, int *ldz, double *work, int *info);

double potentiel(double x)
{
  return 0.5 * x *x;
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
  sys->dx = (double)L/(N-1);
  return 0;
}


void free_all(t_sys *sys)
{
  free(sys->x);
  free(sys->psi);
  free(sys->diag);
  free(sys->z);
  free(sys->o);
  free(sys);
  printf("Everything is freed\n");
}


void print_info(double *d)
{

    printf("\nComputed Energy Levels:\n");
    printf("------------------------------------------------\n");
    printf("Level | Calculated energy | Analytical theory | Error\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < 5; i++) {
        double analytical = i + 0.5; // E = n + 1/2 Quantum harmonic oscillator
        double error = fabs(d[i] - analytical);
        printf("    %d     |  %.6f  |   %.4f     | %.2e\n", 
               i, d[i], analytical, error);
    }
    printf("------------------------------------------------\n");
}

int main(void)
{
  t_sys *sys = malloc(sizeof(t_sys));
  init_stuff(sys);
  double off_diag = -0.5 / (sys->dx*sys->dx);
  // we put alpha = -h²/2m for simplification of the code 
  //double alpha = - (PLANCK_CST * PLANCK_CST)/ (2*MASS); // in our case is alpha = -1/2
  
  // init the gride
  // fixing the factor in diag 
  double k = 1.0 / (sys->dx *sys->dx);
  for (int i = 0; i < N; i++)
  {
    sys->x[i] = -L / 2.0 + i * sys->dx;
    sys->diag[i] = k + potentiel(sys->x[i]);
    if (i < N -1)
      sys->o[i] = off_diag;
  }
  // we need to solve this matrix (see the readme for more mathematical context)
  int n = N;
  int ldz = N;
  char type = 'V'; // the fct dstev_ execpt a pointer to a character (Fortran thing)
  double *work = (double *)malloc((2 * n - 2) * sizeof(double));
  int info;
  printf("Let's gooooo \n");
  dstev_(&type, &n, sys->diag, sys->o, sys->z, &ldz, work, &info);
  if (info != 0)
  {
    printf("Error: LAPACK :%i", info);
    free_all(sys);
    free(work);
    return 1;
  }
  print_info(sys->diag);
    FILE *fp = fopen("wavefunctions.csv", "w");
    if (fp) {
        fprintf(fp, "x,psi_0,psi_1,psi_2,potential_scaled\n");
        
        for (int i = 0; i < N; i++)
        {
            double x = -L / 2.0 + i * sys->dx;            
            fprintf(fp, "%f,%f,%f,%f,%f\n", 
                    x, 
                    sys->z[i + 0*N], // Ground state
                    sys->z[i + 1*N], // 1st Excited state
                    sys->z[i + 2*N], // 2nd Excited state
                    potentiel(x)* 0.1);   
        }
        fclose(fp);
        printf("\nDONE.\n");
    } else {
        printf("Failed to open file for writing.\n");
    }  free_all(sys);
  free(work);
  return 0;
}
