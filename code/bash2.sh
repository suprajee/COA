echo "Exploiting spatial locality of cache"
g++ -fopenmp memory_hit.cpp -o memory_hit
./memory_hit 1000
g++ -fopenmp memory_miss.cpp -o memory_miss
./memory_miss 1000