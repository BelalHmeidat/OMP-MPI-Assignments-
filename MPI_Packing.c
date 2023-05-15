#include <stdio.h>
#include <mpi.h>

int main (int argc, char* argv[]) {

    MPI_Init(&argc, &argv);

    int position, i, myrank; char buff[1000]; float j;
    MPI_Status status;
    position = 0;
    MPI_Comm comm=MPI_COMM_WORLD;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0) {
        i = 27;
        j = 2.7;
        position = 0;
        MPI_Pack(&i, 1, MPI_INT, buff, 1000, &position, MPI_COMM_WORLD);
        MPI_Pack(&j, 1, MPI_FLOAT, buff, 1000, &position , MPI_COMM_WORLD);
        MPI_Bcast(buff, 1000, MPI_PACKED,0 , MPI_COMM_WORLD);

    }


    if (myrank != 0) {
        MPI_Bcast(buff, 1000, MPI_PACKED, 0, MPI_COMM_WORLD);
        // MPI_Recv(buff, 1000, MPI_PACKED, 0, 0, comm, &status);
        //position = 0;
        MPI_Unpack (buff, 1000, &position, &i, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack (buff, 1000, &position, &j, 1, MPI_FLOAT, MPI_COMM_WORLD);
        
        printf("%d\n", i);
        printf("%f\n",j);

    }

    MPI_Finalize();

return 0;

}
