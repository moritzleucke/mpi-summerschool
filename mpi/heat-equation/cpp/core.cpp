// Main solver routines for heat equation solver

#include <mpi.h>

#include "heat.hpp"

// Exchange the boundary values
void exchange(Field& field, const ParallelData parallel)
{

    double* sbuf;
    double* rbuf;
    MPI_Status status;
    // TODO start: implement halo exchange

    // You can utilize the data() method of the Matrix class to obtain pointer
    // to element, e.g. field.temperature.data(i, j)

    // Send to up, receive from down
    sbuf = field.temperature.data(1, 0);
    rbuf = field.temperature.data(field.nx+1, 0);

    // send-recv
    //      MPI_Send(sbuf, field.ny + 2, MPI_DOUBLE, parallel.nup, 123, MPI_COMM_WORLD);
    //      MPI_Recv(rbuf, field.ny + 2, MPI_DOUBLE, parallel.ndown, 123, MPI_COMM_WORLD, &status);
    MPI_Sendrecv(sbuf, field.ny + 2, MPI_DOUBLE, parallel.nup, 123, rbuf, field.ny+2, MPI_DOUBLE, parallel.ndown, 123, MPI_COMM_WORLD, &status);


    // Send to down, receive from up
    sbuf = field.temperature.data(field.nx, 0);
    rbuf = field.temperature.data(0, 0);

    // send-recv
    //      MPI_Send(sbuf, field.ny + 2, MPI_DOUBLE, parallel.ndown, 123, MPI_COMM_WORLD);
    //      MPI_Recv(rbuf, field.ny + 2, MPI_DOUBLE, parallel.nup, 123, MPI_COMM_WORLD, &status);
    MPI_Sendrecv(sbuf, field.ny + 2, MPI_DOUBLE, parallel.ndown, 123, rbuf, field.ny+2, MPI_DOUBLE, parallel.nup, 123, MPI_COMM_WORLD, &status);



    // TODO end
}

void init_communication(Field& field, const ParallelData parallel, MPI_Request* requ)
{

    double* sbuf = field.temperature.data(1, 0);
    double* rbuf = field.temperature.data(field.nx+1, 0);
    MPI_Isend(sbuf, field.ny + 2, MPI_DOUBLE, parallel.nup, 123, MPI_COMM_WORLD, &requ[0]);
    MPI_Irecv(rbuf, field.ny + 2, MPI_DOUBLE, parallel.ndown, 123, MPI_COMM_WORLD, &requ[1]);

    sbuf = field.temperature.data(field.nx, 0);
    rbuf = field.temperature.data(0, 0);
    MPI_Isend(sbuf, field.ny + 2, MPI_DOUBLE, parallel.ndown, 123, MPI_COMM_WORLD, &requ[2]);
    MPI_Irecv(rbuf, field.ny + 2, MPI_DOUBLE, parallel.nup, 123, MPI_COMM_WORLD, &requ[3]);

}

void finalize_communication(MPI_Request* requests)
{
    MPI_Status status[4];
    MPI_Waitall(4, requests, status);
}

void evolve_inner(Field& curr, const Field& prev, const double a, const double dt)
{

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.
  for (int i = 2; i < curr.nx; i++) {
    for (int j = 2; j < curr.ny; j++) {
            curr(i, j) = prev(i, j) + a * dt * (
	       ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
	       ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
               );
    }
  }

}

void evolve_outer(Field& curr, const Field& prev, const double a, const double dt)
{

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.
  for (int i = 1; i < curr.nx + 1; i+=(curr.nx-1)) {
    for (int j = 1; j < curr.ny + 1; j+=(curr.ny-1)) {
            curr(i, j) = prev(i, j) + a * dt * (
	       ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
	       ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
               );
    }
  }

}


// Update the temperature values using five-point stencil */
void evolve(Field& curr, const Field& prev, const double a, const double dt)
{

  // Compilers do not necessarily optimize division to multiplication, so make it explicit
  auto inv_dx2 = 1.0 / (prev.dx * prev.dx);
  auto inv_dy2 = 1.0 / (prev.dy * prev.dy);

  // Determine the temperature field at next time step
  // As we have fixed boundary conditions, the outermost gridpoints
  // are not updated.
  for (int i = 1; i < curr.nx + 1; i++) {
    for (int j = 1; j < curr.ny + 1; j++) {
            curr(i, j) = prev(i, j) + a * dt * (
	       ( prev(i + 1, j) - 2.0 * prev(i, j) + prev(i - 1, j) ) * inv_dx2 +
	       ( prev(i, j + 1) - 2.0 * prev(i, j) + prev(i, j - 1) ) * inv_dy2
               );
    }
  }

}
