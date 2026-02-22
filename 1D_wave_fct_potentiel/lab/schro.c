#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// --- LAPACK PROTOTYPE ---
// We declare the external Fortran routine. 
// Note the underscore '_' which is standard for linking Fortran to C.
extern void dstev_(char *jobz, int *n, double *d, double *e, 
                   double *z, int *ldz, double *work, int *info);

int main() {
    // ==========================================
    // 1. CONFIGURATION
    // ==========================================
    int N = 1000;           // Number of grid points (Resolution)
    double L = 10.0;        // Simulation box size (from -5 to +5)
    double dx = L / (N - 1); // Step size
    
    printf("Initializing Simulation:\n");
    printf("  Grid Points (N): %d\n", N);
    printf("  Box Size (L):    %.2f (%.2f to %.2f)\n", L, -L/2, L/2);
    printf("  Step Size (dx):  %.5f\n\n", dx);

    // ==========================================
    // 2. MEMORY ALLOCATION
    // ==========================================
    // d = Diagonal elements of Hamiltonian
    // e = Off-diagonal elements
    // z = Eigenvectors (columns will store the wavefunctions)
    // work = Workspace array for LAPACK
    double *d = (double *)malloc(N * sizeof(double));
    double *e = (double *)malloc((N - 1) * sizeof(double));
    double *z = (double *)malloc(N * N * sizeof(double));
    double *work = (double *)malloc((2 * N - 2) * sizeof(double));

    if (!d || !e || !z || !work) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // ==========================================
    // 3. BUILD HAMILTONIAN (The Physics)
    // ==========================================
    // Kinetic Energy Constants (Finite Difference Method)
    // T = -1/2 * d^2/dx^2  ->  -1/2 * (y[i+1] - 2y[i] + y[i-1]) / dx^2
    double diag_k = 1.0 / (dx * dx);        // The +2 part / 2
    double off_diag_k = -1.0 / (2.0 * dx * dx); // The -1 part / 2

    // Populate arrays
    for (int i = 0; i < N; i++) {
        // Coordinate x ranges from -L/2 to +L/2
        double x = -L / 2.0 + i * dx;

        // Potential Energy V(x) = 1/2 * x^2 (Harmonic Oscillator)
        // CHANGE THIS LINE to try other potentials (e.g., V=0 for box)
        double V = 0.5 * x * x;

        // Diagonal element = Kinetic (diagonal part) + Potential
        d[i] = diag_k + V;

        // Off-diagonal element (interaction with neighbor)
        if (i < N - 1) {
            e[i] = off_diag_k;
        }
    }

    // ==========================================
    // 4. DIAGONALIZATION (The Solver)
    // ==========================================
    char jobz = 'V'; // 'V' means Compute Eigenvalues AND Eigenvectors
    int n = N;
    int ldz = N;
    int info;

    printf("Solving Hamiltonian matrix using LAPACK (dstev)...\n");
    
    dstev_(&jobz, &n, d, e, z, &ldz, work, &info);

    if (info != 0) {
        printf("Error: LAPACK failed with error code %d\n", info);
        return 1;
    }

    // ==========================================
    // 5. OUTPUT RESULTS
    // ==========================================
    
    // A. Print Energies to Console
    printf("\nComputed Energy Levels:\n");
    printf("------------------------------------------------\n");
    printf("State (n) | Computed E | Analytical E | Error\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < 5; i++) {
        double analytical = i + 0.5; // E = n + 1/2
        double error = fabs(d[i] - analytical);
        printf("    %d     |  %.6f  |   %.4f     | %.2e\n", 
               i, d[i], analytical, error);
    }
    printf("------------------------------------------------\n");

    // B. Write Wavefunctions to CSV for plotting
    // Format: x, psi_0, psi_1, psi_2, V(x)
    FILE *fp = fopen("wavefunctions.csv", "w");
    if (fp) {
        fprintf(fp, "x,psi_0,psi_1,psi_2,potential_scaled\n");
        
        for (int i = 0; i < N; i++) {
            double x = -L / 2.0 + i * dx;
            double V = 0.5 * x * x;
            
            // Eigenvectors are stored in z.
            // z is a 1D array representing a 2D matrix.
            // The j-th eigenvector starts at index [j * N].
            // We scale V by 0.1 just to make it fit on the same graph scale
            
            fprintf(fp, "%f,%f,%f,%f,%f\n", 
                    x, 
                    z[i + 0*N], // Ground state
                    z[i + 1*N], // 1st Excited state
                    z[i + 2*N], // 2nd Excited state
                    V * 0.1);   
        }
        fclose(fp);
        printf("\nWavefunction data saved to 'wavefunctions.csv'.\n");
    } else {
        printf("Failed to open file for writing.\n");
    }

    // Cleanup
    free(d); free(e); free(z); free(work);
    
    return 0;
}
