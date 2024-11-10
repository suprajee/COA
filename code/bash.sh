#!/bin/bash
# Compilation
g++ -fopenmp jacobi_parallel.cpp -o Jacobi_parallel
for Threads in 2 4 8 12 24 
do
    export OMP_NUM_THREADS=$Threads
    echo "For $Threads threads"
    for N in 10 100 10000
    do
        echo "Running Jacobi 2D for N=$N"
        ./Jacobi_parallel $N
    done
    echo ""
done
echo "Sequential"
g++ -fopenmp sequential.cpp -o sequential

    for N in 10 100 10000
    do
        echo "Running Jacobi 2D for N=$N"
        ./sequential $N
    done

