#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


int main(int argc, char *argv[])
{
  int n=1000, reps=10000;

  typedef struct {
    float coords[3];
    int charge;
    char label[2];
  } particle;

  particle particles[n];

  int i, j, myid;
  double t1, t2;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  // Fill in some values for the particles
  if (myid == 0) {
    for (i=0; i < n; i++) {
      for (j=0; j < 3; j++) {
        particles[i].coords[j] = (float)rand()/(float)RAND_MAX*10.0;
      }
      particles[i].charge = 54;
      strcpy(particles[i].label, "Xe");
    }
  }

  // Define datatype for the struct
  MPI_Datatype maeh;
  int blocklengths[3] = {3, 1, 2};
  int types[3] = {MPI_FLOAT, MPI_INT, MPI_CHAR};
  
  MPI_Aint disp[3];
  MPI_Get_address(&particles[0].coords, &disp[0]);
  MPI_Get_address(&particles[0].charge, &disp[1]);
  MPI_Get_address(&particles[0].label, &disp[2]);
  /* Make displacements relative */
  disp[2] -= disp[0];
  disp[1] -= disp[0];
  disp[0] = 0;

  MPI_Type_create_struct(3, blocklengths, disp, types, &maeh);
  MPI_Type_commit(&maeh);

  // Check extent
  // TODO:
  
  MPI_Aint lb, extent;
  MPI_Type_get_extent(maeh, &lb, &extent);
  // if (sizeof(particles[0]) != extent) {
    MPI_Datatype oldtype;
    oldtype = maeh;
    MPI_Type_create_resized(oldtype, 0, sizeof(particles[0]), &maeh);
    MPI_Type_commit(&maeh);
    MPI_Type_free(&oldtype);
  // }

  // Communicate using the created particletype
  // Multiple sends are done for better timing
  t1 = MPI_Wtime();
  if (myid == 0) {
    for (i=0; i < reps; i++) {
      // send 
      MPI_Send(particles, 1, maeh, 1, 123, MPI_COMM_WORLD);
    }
  } else if (myid == 1) {
    for (i=0; i < reps; i++) {
      //  receive
      MPI_Recv(particles, 1, maeh, 0, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
  }
  t2 = MPI_Wtime();

  printf("Time: %i, %e \n", myid, (t2-t1)/(double)reps);
  printf("Check: %i: %s %f %f %f \n", myid, particles[n-1].label,
          particles[n-1].coords[0], particles[n-1].coords[1],
          particles[n-1].coords[2]);

  // Free datatype
  MPI_Type_free(&maeh);

  MPI_Finalize();
  return 0;
}
