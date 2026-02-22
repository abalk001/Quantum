#ifndef BOHR_H
#define BOHR_H


#include <stdio.h>
#include <unistd.h>
#include <math.h>

// Physical constants
# define BOHR_RADIUS 5.29e-11    // m (ground state)
# define ELECTRON_MASS 9.109e-31 // kg
# define ELEMENTARY_CHARGE 1.602e-19 // C
# define EPSILON_0 8.854e-12     // F/m (vacuum permittivity)
# define PLANCK_H 6.626e-34      // J·s
# define SPEED_LIGHT 2.998e8     // m/s
# define RYDBERG_CONST 1.097e7   // m^-1
# define IONIZATION_ENERGY 13.6  // eV
# define N 1000 // number of points in the  wave
# define QUANTUM_N 3 // I guess it the name is enough


// Simulation parameters
#define TIME 1e-15      // fs 
#define DT 1e-18        
#define MAX_LEVELS 5    


// Function prototypes
double	calculate_radius(int n);
double	calculate_velocity(int n);
double	calculate_energy(int n);
void	lyman_series(FILE *spectrum);
void	balmer_series(FILE *spectrum);
void	paschen_series(FILE *spectrum);
void	brackett_series(FILE *spectrum);
double	calculate_wavelength(int n1, int n2);
int		state(int n);
void	wave_fct(void);
void	print_energy_levels(void);

#endif 
