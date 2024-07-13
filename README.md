# Parallel-quicksort_MPI
High Performance Computing Mini Project

Steps to Execute:
Set Up Your Environment:
Install MPI. For Linux, you can use OpenMPI or MPICH. Example for OpenMPI:
sudo apt-get install openmpi-bin openmpi-common libopenmpi-dev

Create the Source File:
Save the provided code in a file named parallel_quicksort.c

Compile the Code:
Open a terminal and compile the code using mpicc:
mpicc -o parallel_quicksort parallel_quicksort.c

Run the Code:
Execute the compiled program using mpirun or mpiexec with the desired number of processes. For example, to run with 4 processes:
mpirun -np 4 ./parallel_quicksort

Evaluate the Performance:
Observe the output for the execution time.
Compare this time with the execution time of a sequential Quicksort algorithm to evaluate the performance enhancement.
Modify the array size (n variable) and the number of processes to see how performance scales.





