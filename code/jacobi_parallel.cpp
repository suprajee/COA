#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <omp.h>

#define MAX_ITERATIONS 100
const double tolerance = 0.0001;

int main(int args, char *argv[])
{

    for (size_t l = 1; l < args; l++)
    {
        int N = atoi(argv[l]);
        double **A = (double **)malloc(N * sizeof(double *));
        double **B = (double **)malloc(N * sizeof(double *));
        for (int i = 0; i < N; i++)
        {
            A[i] = (double *)malloc(N * sizeof(double));
            B[i] = (double *)malloc(N * sizeof(double));
        }

        // std::cout<<N<<"\n";
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                A[i][j] = 30.0;
            }
        }
        for (int i = 0; i < N; i++)
        {
            A[i][0] = A[0][i] = B[i][0] = B[0][i] = 10.0;
            A[i][N - 1] = A[N - 1][i] = B[i][N - 1] = B[N - 1][i] = 140.0;
        }
        // std::cout<<"staert"<<std::endl;;
        auto start_time = omp_get_wtime();
        double difmax = 1000.0;
        for (int i = 0; i < MAX_ITERATIONS && difmax > tolerance; i++)
        {
            // std::cout<<i<<std::endl;
            difmax = 0;
#pragma omp parallel for collapse(2)
            for (int j = 1; j < N - 1; j++)
            {
                for (int k = 1; k < N - 1; k++)
                {
                    B[j][k] = 0.2 * (A[j][k] + A[j][k - 1] + A[j - 1][k] + A[j + 1][k] + A[j][k + 1]);
                    if (fabs(B[j][k] - A[j][k]) > difmax)
                    {
                        difmax = fabs(B[j][k] - A[j][k]);
                    }
                }
            }
// std::cout<<difmax<<std::endl;
#pragma omp barrier
            std::swap(A, B);
        }
        auto end_time = omp_get_wtime();
        std::cout << "Time in parallel running of this code is " << std::fixed << std::setprecision(6) << end_time - start_time << '\n';
    }
}