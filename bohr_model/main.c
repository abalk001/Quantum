#include "bohr.h"



int main()
{
  if (ground_state())
    return(-1);
  
  FILE *spectrum = fopen("spectrum.csv", "w");
  if (!spectrum)
    return(-1);
  
  fprintf(spectrum,"n1,n2,wavelength_nm,energy_eV,series\n");
  lyman_series(spectrum);
  balmer_series(spectrum);
  paschen_series(spectrum);
  brackett_series(spectrum);
  wave_fct();
  fclose(spectrum);
  print_energy_levels();
  return 0;
}
