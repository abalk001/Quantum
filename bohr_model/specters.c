#include "bohr.h"

void lyman_series(FILE *spectrum)
{
	for (int n2 = 2; n2 <= 6; n2++)
	{
		double lambda = calculate_wavelength(1, n2);
		double delta_E = calculate_energy(1) - calculate_energy(n2);
		fprintf(spectrum, "1,%d,%.2f,%.4f,Lyman\n", n2, lambda, -delta_E);
	}
}
void balmer_series(FILE *spectrum)
{
	for (int n2 = 3; n2 <= 7; n2++)
	{
		double lambda = calculate_wavelength(2, n2);
		double delta_E = calculate_energy(2) - calculate_energy(n2);
		fprintf(spectrum, "2,%d,%.2f,%.4f,Balmer\n", n2, lambda, -delta_E);
	}
}
void paschen_series(FILE *spectrum)
{
    for (int n2 = 4; n2 <= 7; n2++) {
        double lambda = calculate_wavelength(3, n2);
        double delta_E = calculate_energy(3) - calculate_energy(n2);
        fprintf(spectrum, "3,%d,%.2f,%.4f,Paschen\n", n2, lambda, -delta_E);
    }
}
void brackett_series(FILE *spectrum)
{
    for (int n2 = 5; n2 <= 8; n2++) {
        double lambda = calculate_wavelength(4, n2);
        double delta_E = calculate_energy(4) - calculate_energy(n2);
        fprintf(spectrum, "4,%d,%.2f,%.4f,Brackett\n", n2, lambda, -delta_E);
    }
}