#include <iostream>
#include <mpi.h>


int main(int argc, char *argv[])
{
    int size, rank, resultlen;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &resultlen);
    std::cout << "we are " << size << " processes in total!" << std::endl;
    
    // put everything out of the buffer
    flush(std::cout);
    // wait for the others
    MPI_Barrier(MPI_COMM_WORLD);

    std::cout << "Hello! I am rank " << rank << " on node " << name << "!" << std::endl;
    
    // point-to-point test
    double buffer[3];
    if (rank == 0) {
        for (int i=0; i<3; i++) buffer[i]=i;
        MPI_Send(buffer, 3, MPI_DOUBLE, 1, 123, MPI_COMM_WORLD);
    } else {
        MPI_Status status;
        MPI_Recv(buffer, 3, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &status);
        for (int i=0; i<3; i++) std::cout << buffer[i] << std::endl;
    }

    MPI_Finalize();
}
