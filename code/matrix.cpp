#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <omp.h>

const int MAX_ITERATIONS = 100;
const double tolerance = 0.0001;

int main(int args, char *argv[])
{
    std::cout << std::fixed << std::setprecision(6);

    for (int l = 1; l < args; l++)
    {
        int N = atoi(argv[l]);
        double **A = (double **)malloc(N * sizeof(double *));
        double **B = (double **)malloc(N * sizeof(double *));
        double **C = (double **)malloc(N * sizeof(double *));
        double **D = (double **)malloc(N * sizeof(double *));
        for (int i = 0; i < N; i++)
        {
            A[i] = (double *)malloc(N * sizeof(double));
            B[i] = (double *)malloc(N * sizeof(double));
            C[i] = (double *)malloc(N * sizeof(double));
            D[i] = (double *)malloc(N * sizeof(double));
        }
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                A[i][j] = C[i][j] = 30.0;
            }
        }
        for (int i = 0; i < N; i++)
        {
            A[i][0] = A[0][i] =B[i][0] = B[0][i] = C[i][0] = C[0][i] = 10.0;
            A[i][N - 1] = A[N - 1][i] = B[i][N-1] = B[N-1][i] = C[i][N - 1] = C[N - 1][i] = 140.0;
        }
        std::cout<<"Before any operations :\n";
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                std::cout << A[j][k] << ' ';
            }
            std::cout << '\n';
        }

        auto start_time = omp_get_wtime();
        double difmax = 1000.0;
        
        for (int i = 0; i < MAX_ITERATIONS && difmax > tolerance; i++)
        {
            difmax = 0;
#pragma omp parallel for collapse(2)
            for (int j = 1; j < N - 1; j++)
            {
                for (int k = 1; k < N - 1; k++)
                {
                    B[j][k] = 0.2 * (A[j][k] + A[j][k - 1] + A[j - 1][k] + A[j + 1][k] + A[j][k + 1]);
                    if(fabs(B[j][k]-A[j][k])>difmax){
                        difmax=fabs(B[j][k]-A[j][k]);
                    }
                }
            }
#pragma omp barrier
            std::swap(A, B);
        }
        std::cout << "matrix with parallelism\n";
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                std::cout << std::fixed << std::setprecision(6) << A[j][k] << ' ';
                std::swap(A[j][k], C[j][k]);
            }
            std::cout << "\n";
        }
        difmax = 1000.0;
        for (int i = 0; i < MAX_ITERATIONS && difmax > tolerance; i++)
        {
            difmax = 0;
            for (int j = 1; j < N - 1; j++)
            {
                for (int k = 1; k < N - 1; k++)
                {
                    B[j][k] = 0.2 * (A[j][k] + A[j][k - 1] + A[j - 1][k] + A[j + 1][k] + A[j][k + 1]);
                    if(fabs(B[j][k]-A[j][k])>difmax){
                        difmax=fabs(B[j][k]-A[j][k]);
                    }
                }
            }
            std::swap(A, B);
        }
        std::cout << "matrix without parallelism\n";
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                std::cout << std::fixed << std::setprecision(6) << A[j][k] << ' ';
            }
            std::cout << "\n";
        }
        auto end_time = omp_get_wtime();
        std::cout << "Serial - Parallel : \n";
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                std::cout << C[i][j] - A[i][j] << ' ';
            }
            std::cout << std::endl;
        }
    }
}