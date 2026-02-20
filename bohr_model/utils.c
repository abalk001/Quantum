#include "bohr.h"

double calculate_radius(int n)
{
    return BOHR_RADIUS * n * n;
}

double calculate_velocity(int n)
{
    return (ELEMENTARY_CHARGE * ELEMENTARY_CHARGE) / 
           (2.0 * EPSILON_0 * PLANCK_H * n);
}
double calculate_energy(int n) 
{
    return -IONIZATION_ENERGY / (n * n);
}
double calculate_wavelength(int n1, int n2)
{
    if (n2 <= n1)
        return 0;
    double inv_lambda = RYDBERG_CONST * (1.0 / (n1*n1) - 1.0 / (n2*n2)); // lambda = Ry * (1/n1^2 - 1/n2^2)
    return 1.0 / inv_lambda * 1e9; // convert to nm
}
int state(int n)
{
	FILE *f = fopen("data.csv", "w");
	if (!f)
		return(-1);

	fprintf(f,"time,n,x,y,radius,velocity,energy\n");
    double radius = calculate_radius(n);
    double velocity = calculate_velocity(n);
    double omega = velocity / radius; 
    double energy = calculate_energy(n);
	double t = 0;
    for (int i = 0; i < (int)(TIME/DT); i++)
    {
        double x = radius * cos(omega * t);
        double y = radius * sin(omega * t);
        fprintf(f, "%.15e,%d,%.15e,%.15e,%.15e,%.15e,%.6f\n",
                t, n, x, y, radius, velocity, energy);
        t += DT;
    }
    fclose(f);
	return 0;
}

void print_energy_levels()
{
    // Print energy levels to console
    printf("\n=== BOHR MODEL - HYDROGEN ENERGY LEVELS ===\n\n");
    printf("Level n | Radius (m)    | Velocity (m/s) | Energy (eV)\n");
    printf("--------|---------------|----------------|-------------\n");
    for (int level = 1; level <= MAX_LEVELS; level++) {
        double r = calculate_radius(level);
        double v = calculate_velocity(level);
        double e = calculate_energy(level);
        printf("   %d    | %.4e    | %.4e     | %.4f\n", level, r, v, e);
    }
    
    printf("\n=== HYDROGEN SPECTRUM (Selected Lines) ===\n\n");
    printf("Balmer Series (Visible Light):\n");
    printf("Transition | Wavelength (nm) | Energy (eV) | Color\n");
    printf("-----------|-----------------|-------------|------------\n");
    printf("  3 → 2    | %.1f           | %.3f       | Red (Hα)\n", 
           calculate_wavelength(2,3), -(calculate_energy(2)-calculate_energy(3)));
    printf("  4 → 2    | %.1f           | %.3f       | Cyan (Hβ)\n", 
           calculate_wavelength(2,4), -(calculate_energy(2)-calculate_energy(4)));
    printf("  5 → 2    | %.1f           | %.3f       | Blue (Hγ)\n", 
           calculate_wavelength(2,5), -(calculate_energy(2)-calculate_energy(5)));
    printf("  6 → 2    | %.1f           | %.3f       | Violet (Hδ)\n", 
           calculate_wavelength(2,6), -(calculate_energy(2)-calculate_energy(6)));
    
    printf("\nData written to data.csv and spectrum.csv\n\n");
}