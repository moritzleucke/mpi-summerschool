#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank;
    int array[8][6];
    MPI_Datatype vector;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize arrays
    if (rank == 0) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 6; j++) {
                array[i][j] = (i + 1) * 10 + j + 1;
            }
        }
    } else {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 6; j++) {
                array[i][j] = 0;
            }
        }
    }

    // Print data on rank 0
    if (rank == 0) {
        printf("Data on rank %d\n", rank);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 6; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    // Create datatype
    // TODO
    MPI_Datatype wau, wauwau;
    MPI_Type_vector(8, 1, 6, MPI_INT, &wau);
    MPI_Type_create_resized(wau, 0, sizeof(int), &wauwau);
    MPI_Type_commit(&wauwau);


    // Send data from rank 0 to rank 1
    // TODO
    if (rank == 0) {
        MPI_Send(&array[0][0], 2, wauwau, 1, 123, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&array[0][0], 2, wauwau, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Free datatype
    // TODO
    MPI_Type_free(&wau);

    // Print received data
    if (rank == 1) {
        printf("Received data on rank %d\n", rank);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 6; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}
