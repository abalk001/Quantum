#include <stdio.h>
#include <unistd.h>
#include <math.h>

# define BOHR 1
# define TIME 20 // sec 
# define DT 0.1
# define RADIUS 1
# define V1 2.19e6

int main()
{
  
  FILE *f = fopen("data.csv", "w");
  if (!f)
    return(-1);
  double t = 0;
  double x = 0;
  double y = 0;
  fprintf(f,"time,x,y\n");
  for (int i = 0; i < TIME/DT; i++)
  {
    x = RADIUS * cos(V1 * t);
    y = RADIUS * sin(V1 * t);
    fprintf(f, "%.2f,%.2f,%.2f\n",t,x,y);
    t +=DT;
  }
  fclose(f);
  return 0;
}
