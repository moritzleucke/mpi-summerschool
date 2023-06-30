#include <cstdio>
#include <vector>
#include <mpi.h>

#define NTASKS 4

void init_buffers(std::vector<int> &sendbuffer, std::vector<int> &recvbuffer);
void print_buffers(std::vector<int> &buffer);


int main(int argc, char *argv[])
{
    int ntasks, rank;
    std::vector<int> sendbuf(2 * NTASKS), recvbuf(2 * NTASKS);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (ntasks != NTASKS) {
        if (rank == 0) {
            fprintf(stderr, "Run this program with %i tasks.\n", NTASKS);
        }
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    /* Initialize message buffers */
    init_buffers(sendbuf, recvbuf);

    /* Print data that will be sent */
    print_buffers(sendbuf);

    /* TODO: use a single collective communication call
     *       (and maybe prepare some parameters for the call)
     */

    // CASE 1
    //      if (rank == 0) recvbuf = sendbuf;
    //      MPI_Bcast(recvbuf.data(), 2*ntasks, MPI_INT, 0, MPI_COMM_WORLD);

    // CASE 2
    //      MPI_Scatter(sendbuf.data(), 2, MPI_INT, recvbuf.data(), 2, MPI_INT, 0, MPI_COMM_WORLD);

    // CASE 3
    //      const int recvcounts[4] = {1, 1, 2, 4};
    //      const int displs[4] = {0, 1, 2, 4};
    //      MPI_Gatherv(sendbuf.data(), recvcounts[rank], MPI_INT, recvbuf.data(), recvcounts, displs, MPI_INT, 1, MPI_COMM_WORLD);

    // CASE 4
    MPI_Alltoall(sendbuf.data(), 2, MPI_INT, recvbuf.data(), 2, MPI_INT, MPI_COMM_WORLD);

    /* Print data that was received */
    /* TODO: use correct buffer */
    print_buffers(recvbuf);

    MPI_Finalize();
    return 0;
}


void init_buffers(std::vector<int> &sendbuffer, std::vector<int> &recvbuffer)
{
    int rank;
    int buffersize = sendbuffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < buffersize; i++) {
        recvbuffer[i] = -1;
        sendbuffer[i] = i + buffersize * rank;
    }
}


void print_buffers(std::vector<int> &buffer)
{
    int rank, ntasks;
    int buffersize = buffer.size();

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    std::vector<int> printbuffer(buffersize * ntasks);

    MPI_Gather(buffer.data(), buffersize, MPI_INT,
               printbuffer.data(), buffersize, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int j = 0; j < ntasks; j++) {
            printf("Task %2i:", j);
            for (int i = 0; i < buffersize; i++) {
                printf(" %2i", printbuffer[i + buffersize * j]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
