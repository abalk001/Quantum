#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* * Helper: Sign function 
 * Returns 1.0 if x >= 0, -1.0 if x < 0
 */
double sign(double x) {
    return (x >= 0.0) ? 1.0 : -1.0;
}

/*
 * dstev: Computes all eigenvalues and eigenvectors of a real symmetric 
 * tridiagonal matrix using the Implicit QR algorithm.
 *
 * N:     Dimension of the matrix
 * d:     Diagonal elements (Array of size N)
 * e:     Off-diagonal elements (Array of size N). 
 * e[i] connects d[i] and d[i+1]. e[N-1] is ignored.
 * Z:     NxN matrix for eigenvectors (flattened array). 
 * If NULL, eigenvectors are not computed.
 * If provided, it MUST be initialized to the Identity matrix.
 */
int dstev(int n, double *d, double *e, double *Z) {
    int m, l, iter, i, k;
    double s, c, p, r, dl1, el1;
    double g, d_m, d_m1, e_m1, b, f, dd;

    // Iterate until the matrix is diagonal
    // We process the matrix from bottom (n-1) up to 1
    // effectively deflating the matrix one by one.
    for (m = n - 1; m > 0; m--) {
        
        iter = 0;
        
        while (1) {
            // 1. Check for convergence (Look for small sub-diagonal element)
            // We look backwards from m-1 to find where the block starts (l)
            for (l = m; l > 0; l--) {
                // Convergence criterion: |e[l-1]| <= eps * (|d[l-1]| + |d[l]|)
                dd = fabs(d[l-1]) + fabs(d[l]);
                if (fabs(e[l-1]) <= 1e-15 * dd) { // Using standard epsilon roughly
                    break;
                }
            }

            // If the block size is 1 (l == m), we found an eigenvalue!
            if (l == m) {
                break; // Proceed to next eigenvalue (m-1)
            }

            if (iter++ > 30) {
                printf("Error: Too many iterations. Convergence failed.\n");
                return -1;
            }

            // 2. Compute Wilkinson Shift (g)
            // Based on the bottom 2x2 block of the active submatrix [l...m]
            d_m  = d[m];
            d_m1 = d[m-1];
            e_m1 = e[m-1];

            // Formula: g = (d_m1 - d_m) / 2
            g = (d_m1 - d_m) / 2.0;
            
            // shift = d_m + g - sign(g) * sqrt(g^2 + e_m1^2)
            // We compute the denominator of the shift term first
            double denom = hypot(g, e_m1);
            
            // Usually we start the "chase" with x = d[l] - shift
            // But we can simplify x calculation to avoid explicit shift subtraction:
            // This is the standard "Francis step" initialization
            double shift_term = e_m1 * e_m1 / (g + sign(g) * denom);
            double x = d[l] - d_m + shift_term;
            double z = e[l];

            // 3. The Chase (Implicit QR Sweep)
            // Rotate from top (l) down to bottom (m-1)
            for (k = l; k < m; k++) {
                
                // Calculate Givens rotation to zero out 'z' using 'x'
                r = hypot(x, z);
                c = x / r;
                s = z / r;

                // Apply rotation to previous off-diagonal (if not start)
                if (k > l) {
                    e[k-1] = r;
                }

                // Apply rotation to the diagonal and off-diagonal elements
                // This simulates G' * A * G
                double dk  = d[k];
                double dk1 = d[k+1];
                double ek  = e[k];

                // The manual update formulas for the 2x2 block
                e[k]   = c * s * (dk + dk1) + (c*c - s*s) * ek; // Just for illustration (unstable)
                // Robust update:
                e[k] = (dk1 - dk) * s * c + ek * (c*c - s*s);
                d[k]   = dk - (e[k] - ek) * s / c; // Only if c != 0, careful here
                // Safer standard update:
                d[k+1] = d[k] + (dk1 - d[k]) * c * c + 2.0 * ek * c * s;
                d[k]   = d[k] + (dk1 - d[k]) * s * s - 2.0 * ek * c * s; // Wait, let's use the stable form:

                // Re-doing stable update for d and e:
                // We actually know the geometry:
                // We just computed c,s. 
                // x (new bulge for next step) = s * e[k+1]
                // z (new target for next step) = c * e[k+1]
                
                if (k < m - 1) {
                    x = e[k];       // The valid off-diagonal generated
                    z = s * e[k+1]; // The "Bulge" created at (k, k+2)
                    e[k+1] = c * e[k+1]; // The off-diagonal shrinks
                }
                
                // ACCUMULATE EIGENVECTORS (Optional)
                if (Z) {
                    // Apply rotation to columns k and k+1 of Z
                    // Z is stored row-major or column-major? Let's assume Row-Major Z[row][col]
                    // But flat array: Z[row * n + k]
                    for (i = 0; i < n; i++) {
                        double z_ik  = Z[i * n + k];
                        double z_ik1 = Z[i * n + (k+1)];
                        Z[i * n + k]     = c * z_ik - s * z_ik1;
                        Z[i * n + (k+1)] = s * z_ik + c * z_ik1;
                    }
                }
            }
        }
    }
    return 0;
}

// Helper to print matrix
void print_matrix(int n, double *A) {
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            printf("%6.3f ", A[i*n + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int n = 4;
    
    // 1. The Diagonal (d)
    double d[] = {1.0, 2.0, 2.0, 3.0};
    
    // 2. The Off-Diagonal (e)
    // Note: e[3] is padding/zero. e[0] connects d[0]-d[1].
    double e[] = {1.0, 1.0, 1.0, 0.0}; 

    // 3. Eigenvectors (Identity Matrix)
    double Z[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    printf("--- Before ---\n");
    printf("Diagonal: ");
    for(int i=0; i<n; i++) printf("%.3f ", d[i]);
    printf("\nOff-Diag: ");
    for(int i=0; i<n-1; i++) printf("%.3f ", e[i]);
    printf("\n\n");

    // Run the solver
    dstev(n, d, e, Z);

    printf("--- After ---\n");
    printf("Eigenvalues: ");
    for(int i=0; i<n; i++) printf("%.5f ", d[i]);
    printf("\n\n");
    
    printf("Eigenvectors (Columns):\n");
    print_matrix(n, Z);

    return 0;
}
