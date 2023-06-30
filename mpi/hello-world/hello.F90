program hello
  use mpi_f08
  implicit none
  
  integer :: size, rank, returnlen
  character*(MPI_MAX_PROCESSOR_NAME) name

  call MPI_INIT()
  call MPI_COMM_size(MPI_COMM_WORLD, size)
  call MPI_COMM_rank(MPI_COMM_WORLD, rank)
  call MPI_Get_processor_name(name, returnlen)
  
  print *, 'Hello, I am rank ', rank , ' of ', size, ' on node ', name
  call MPI_FINALIZE()

end program hello
