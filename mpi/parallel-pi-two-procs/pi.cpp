#include <cstdio>
#include <iostream>
#include <cmath>
#include <mpi.h>

constexpr int n = 840;

int main(int argc, char** argv)
{
  int myid, ntasks;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  printf("Computing approximation to pi with N=%d\n", n);

  int istart = myid * n/ntasks + 1;
  int istop = (myid + 1) * n/ntasks;
  std::cout << istart << ",  " << istop << std::endl;

  double pi = 0.0;
  for (int i=istart; i <= istop; i++) {
    double x = (i - 0.5) / n;
    pi += 1.0 / (1.0 + x*x);
  }

  if (myid == 0) {
    MPI_Send(&pi, 1, MPI_DOUBLE, 1, 123, MPI_COMM_WORLD);
  } else {
    double pi_aux;
    MPI_Recv(&pi_aux, 1, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &status);
    pi += pi_aux;
  }

  if (myid == 1) {
    pi *= 4.0 / n;
    printf("Approximate pi=%18.16f (exact pi=%10.8f)\n", pi, M_PI);
  }

  MPI_Finalize();
}
