#include <iostream>
#include <cmath>
#include <iomanip>
#include <omp.h>

#define MAX_ITERATIONS 10000
const double tolerance = 0.0001;

void jacobi_columnwise(double **A, double **B, int N)
{
    double difmax = tolerance + 1;
    for (int iter = 0; iter < MAX_ITERATIONS && difmax > tolerance; iter++)
    {
        difmax = 0.0;
#pragma omp parallel for collapse(2)
        for (int j = 1; j < N - 1; j++)
        {
            for (int i = 1; i < N - 1; i++)
            {
                B[i][j] = 0.25 * (A[i - 1][j] + A[i + 1][j] + A[i][j - 1] + A[i][j + 1]);
                if (fabs(B[i][j] - A[i][j]) > difmax)
                {
                    difmax = fabs(B[i][j] - A[i][j]);
                }
            }
        }
#pragma omp barrier
        std::swap(A, B);
    }
}

int main(int argc, char *argv[])
{
    for (int l = 1; l < argc; l++)
    {
        int N = atoi(argv[l]);

        // Allocate 2D arrays dynamically
        double **A = new double *[N];
        double **B = new double *[N];
        for (int i = 0; i < N; i++)
        {
            A[i] = new double[N];
            B[i] = new double[N];
        }

        // Initialize matrix boundaries
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                A[i][j] = 30.0;
                B[i][j] = 30.0;
            }
            A[i][0] = A[0][i] = B[i][0] = B[0][i] = 10.0;
            A[i][N - 1] = A[N - 1][i] = B[i][N - 1] = B[N - 1][i] = 140.0;
        }

        auto start_time = omp_get_wtime();
        jacobi_columnwise(A, B, N);
        auto end_time = omp_get_wtime();

        std::cout << "Time in column-wise parallel version with N=" << N << ": "
                  << std::fixed << std::setprecision(6) << end_time - start_time << " seconds\n";

        // Free allocated memory
        for (int i = 0; i < N; i++)
        {
            delete[] A[i];
            delete[] B[i];
        }
        delete[] A;
        delete[] B;
    }

    return 0;
}
