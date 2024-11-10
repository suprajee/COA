g++ -fopenmp schedule_static.cpp -o schedule_static
./schedule_static 10000

g++ -fopenmp schedule_dynamic.cpp -o schedule_dynamic
./schedule_dynamic 10000

g++ -fopenmp schedule_guided.cpp -o schedule_guided
./schedule_guided 10000