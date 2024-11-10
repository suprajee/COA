#include <iostream>
#include <cmath>
#include <iomanip>
#include <omp.h>
#include <cstdlib>

#define MAX_ITERATIONS 100
const double tolerance = 0.0001;
#define TILE_SIZE 64   // Tile size for tiling algorithm

int main(int args, char *argv[]) {
    for (size_t l = 1; l < args; l++) {
        int N = atoi(argv[l]);

        // Allocate matrices dynamically
        double **A = (double **)malloc(N * sizeof(double *));
        double **B = (double **)malloc(N * sizeof(double *));
        for (int i = 0; i < N; i++) {
            A[i] = (double *)malloc(N * sizeof(double));
            B[i] = (double *)malloc(N * sizeof(double));
        }

        // Initialize matrices
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = 30.0;
            }
        }

        // Set boundary conditions
        for (int i = 0; i < N; i++)  {
            A[i][0] = B[i][0] = 10.0;
            A[i][N - 1] = B[i][N - 1] = 140.0;
            A[0][i] = B[0][i] = 10.0;
            A[N - 1][i] = B[N - 1][i] = 140.0;
        }

        // Measure time for Jacobi without tiling
        auto start_time = omp_get_wtime();
        double difmax = 1000.0;
        int iteration = 0;

        // Non-tiled Jacobi iteration
        while (iteration < MAX_ITERATIONS && difmax > tolerance) {
            difmax = 0.0;
#pragma omp parallel for collapse(2) reduction(max:difmax)
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < N - 1; j++) {
                    B[i][j] = 0.25 * (A[i - 1][j] + A[i + 1][j] + A[i][j - 1] + A[i][j + 1]);
                    double diff = fabs(B[i][j] - A[i][j]);
                    if (diff > difmax) {
                        difmax = diff;
                    }
                }
            }
            std::swap(A, B);
            iteration++;
        }
        auto end_time = omp_get_wtime();
        double time_without_tiling = end_time - start_time;
        std::cout << "Time for Jacobi without tiling: " << std::fixed << std::setprecision(6)
                  << time_without_tiling << " seconds\n";

        // Re-initialize matrices for consistency
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() / (double)RAND_MAX;
                B[i][j] = 0.0;
            }
        }

        // Set boundary conditions again
        for (int i = 0; i < N; i++) {
            A[i][0] = B[i][0] = 10.0;
            A[i][N - 1] = B[i][N - 1] = 140.0;
            A[0][i] = B[0][i] = 10.0;
            A[N - 1][i] = B[N - 1][i] = 140.0;
        }

        // Measure time for Jacobi with tiling
        start_time = omp_get_wtime();
        difmax = 1000.0;
        iteration = 0;

        // Tiled Jacobi iteration
        while (iteration < MAX_ITERATIONS && difmax > tolerance) {
            difmax = 0.0;
#pragma omp parallel for collapse(2) reduction(max:difmax)
            for (int ii = 1; ii < N - 1; ii += TILE_SIZE) {
                for (int jj = 1; jj < N - 1; jj += TILE_SIZE) {
                    // Iterate over the tile
                    for (int i = ii; i < std::min(ii + TILE_SIZE, N - 1); i++) {
                        for (int j = jj; j < std::min(jj + TILE_SIZE, N - 1); j++) {
                            B[i][j] = 0.25 * (A[i - 1][j] + A[i + 1][j] + A[i][j - 1] + A[i][j + 1]);
                            double diff = fabs(B[i][j] - A[i][j]);
                            if (diff > difmax) {
                                difmax = diff;
                            }
                        }
                    }
                }
            }
            std::swap(A, B);
            iteration++;
        }
        end_time = omp_get_wtime();
        double time_with_tiling = end_time - start_time;
        std::cout << "Time for Jacobi with tiling: " << std::fixed << std::setprecision(6)
                  << time_with_tiling << " seconds\n";

        // Display performance improvement
        double improvement = (time_without_tiling - time_with_tiling) / time_without_tiling * 100.0;
        std::cout << "Performance improvement with tiling: " << std::fixed << std::setprecision(2)
                  << improvement << "%\n";

        // Free allocated memory
        for (int i = 0; i < N; i++) {
            free(A[i]);
            free(B[i]);
        }
        free(A);
        free(B);
    }

    return 0;
}