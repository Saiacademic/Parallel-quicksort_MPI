#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

void parallel_quicksort(int* arr, int low, int high, int depth, int rank, int numprocs) {
    if (depth == 0 || numprocs == 1) {
        quicksort(arr, low, high);
        return;
    }

    int pivot_index = partition(arr, low, high);

    int partner = rank + (1 << (depth - 1));
    if (partner < numprocs) {
        MPI_Send(arr + pivot_index + 1, high - pivot_index, MPI_INT, partner, 0, MPI_COMM_WORLD);
        parallel_quicksort(arr, low, pivot_index - 1, depth - 1, rank, numprocs);
        MPI_Recv(arr + pivot_index + 1, high - pivot_index, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        parallel_quicksort(arr, low, pivot_index - 1, depth - 1, rank, numprocs);
        parallel_quicksort(arr, pivot_index + 1, high, depth - 1, rank, numprocs);
    }
}

int main(int argc, char* argv[]) {
    int numprocs, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 100000;  // Example array size
    int* arr = (int*)malloc(n * sizeof(int));

    if (rank == 0) {
        srand(0);
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % n;
        }
    }

    MPI_Bcast(arr, n, MPI_INT, 0, MPI_COMM_WORLD);

    int depth = 0;
    while ((1 << depth) < numprocs) depth++;

    double start_time = MPI_Wtime();
    parallel_quicksort(arr, 0, n - 1, depth, rank, numprocs);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Time taken: %f seconds\n", end_time - start_time);
    }

    free(arr);
    MPI_Finalize();
    return 0;
}
