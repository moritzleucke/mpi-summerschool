#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int rank;
    int array[8][8];

    // Declare a variable storing the MPI datatype
    // TODO

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize arrays
    if (rank == 0) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                array[i][j] = (i + 1) * 10 + j + 1;
            }
        }
    } else {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                array[i][j] = 0;
            }
        }
    }

    // Print data on rank 0
    if (rank == 0) {
        printf("Data on rank %d\n", rank);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    // Create datatype
    // TODO

    // MPI_Datatype row;
    // MPI_Type_vector(8, 1, 8, MPI_INT, &row);
    // MPI_Type_commit(&row);
    // if (rank == 0){
    //     MPI_Send(&array[0][1], 1, row, 1, 123, MPI_COMM_WORLD);
    // } else {
    //     MPI_Recv(&array[0][1], 1, row, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // }
    // MPI_Type_free(&row);

    //      MPI_Datatype weird;
    //      int blocklens[4] = {1, 2, 3, 4};
    //      int displs[4] = {0, 17, 34, 51};
    //      MPI_Type_indexed(4, blocklens, displs, MPI_INT, &weird);
    //      MPI_Type_commit(&weird);
    //      if (rank == 0){
    //          MPI_Send(array, 1, weird, 1, 123, MPI_COMM_WORLD);
    //      } else {
    //          MPI_Recv(array, 1, weird, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //      }   
    //      MPI_Type_free(&weird);

    MPI_Datatype muh;
    int sizes[2] = {8, 8};
    int subsizes[2] = {4, 4};
    int offset[2] = {2, 2};
    MPI_Type_create_subarray(2, sizes, subsizes, offset, MPI_ORDER_C, MPI_INT, &muh);
    MPI_Type_commit(&muh);
    if (rank == 0) {
        MPI_Send(array, 1, muh, 1, 123, MPI_COMM_WORLD);
    } else {
        MPI_Recv(array, 1, muh, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    MPI_Type_free(&muh);


    // Print received data
    if (rank == 1) {
        printf("Received data on rank %d\n", rank);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                printf("%3d", array[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}
