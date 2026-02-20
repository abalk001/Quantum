#include "bohr.h"

void wave_fct()
{
	FILE *file = fopen("wavefct.csv", "w");
	if (!file)
	{
		fprintf(stderr,"error in wavefct.csv file");
		return;
	}
	double x[N];
	double real_psi = 0;
	double image_psi = 0;
	double prob_dens = 0;
	double t = 0;

	for (int i = 0; i < N; i++)
		x[i] = (2*M_PI*BOHR_RADIUS *i) / (double)N;
	double energy = ((PLANCK_H * PLANCK_H) * (QUANTUM_N*QUANTUM_N))/(2*ELECTRON_MASS*BOHR_RADIUS*BOHR_RADIUS);
	int max_steps = (int)TIME/DT;
	double norm = 1.0 / sqrt(2.0 * M_PI * BOHR_RADIUS);
	for (int j = 0; j < max_steps; j++)
	{
		for(int i = 0; i < N;i++)
		{
		double k =  QUANTUM_N / BOHR_RADIUS;
		double omega = energy / PLANCK_H;
		double theta = (k * x[i]) - (omega * t);
		real_psi = norm * cos(theta);
		image_psi = norm * sin(theta);
		prob_dens = real_psi * real_psi + image_psi * image_psi;
		fprintf(file, "%lf,%lf,%lf,%lf,%lf\n", t, x[i], real_psi, image_psi, prob_dens);
		}
		t += DT;
	} 
	fclose (file);
}