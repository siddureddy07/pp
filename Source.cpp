#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <iostream>
using namespace std;

int isPrime(int number) {
    if (number <= 1)
        return 0;

    for (int i = 2; i <= sqrt(number); i++) {
        if (number % i == 0)
            return 0;
    }

    return 1;
}

int main(int argc, char** argv) {
    int rank, size;
    int question[4][4]{};
    int row[4];
    int ele = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (rank == 0) {
        printf("Enter the matrix elements:\n");
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cin >> question[i][j];
            }
        }
    }

    // Scatter the matrix to all processes
    MPI_Scatter(question, 4 , MPI_INT, row, 4 , MPI_INT, 0, MPI_COMM_WORLD);

    // Replace prime and non-prime elements
    cout << rank;
    for (int i = 0; i < 4 ; i++) {
        if (isPrime(row[i])) {
           row[i] = 1;
        }
        else {
            row[i] = 0;
        }
    }

    // Gather the modified matrix to the root process
    MPI_Gather(row, 4 , MPI_INT, question, 4 , MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nModified Matrix:\n");
        
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                printf("%d ", question[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}