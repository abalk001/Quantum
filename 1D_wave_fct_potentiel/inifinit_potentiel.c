#include "potentiel.h"

void inifinit_potentiel()
{
	FILE *f = fopen("potentiel.csv", "w");
	if (f == NULL) {
		fprintf(stderr, "Error opening file\n");
		return;
	}

	double x[N];
	for (int i = 0; i < N; i++)
		x[i] = i * ((double)L / (double)N);
	double norm = sqrt(2/(double)L);
	fprintf(f,"x,psi(x)\n");
	for (int i = 0; i < N; i++)
	{
		double psi = norm * sin(M_PI * QUANTUM_N * x[i] / (double)L);
		fprintf(f, "%f,%f\n", x[i], psi);
	}
	fclose(f);
}
