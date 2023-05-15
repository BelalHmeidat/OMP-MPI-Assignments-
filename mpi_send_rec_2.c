#include <stdio.h>
#include <mpi.h>
#include <math.h>


void genrate_matrix(int randBuff [], int N){
    int i;
    for (i = 0; i < N; i++){
       randBuff[i] = rand() % N;
       printf("%d ", randBuff[i]);
    }
}

int main (int argc, char *argv[]) {

    MPI_Init(&argc, &argv);
    int selectedSending, selectedRecieving; //the selected number to be sent to the next process
    int rank, size, N; 
    int i, j; 
    char message [100];
    int winFlag = 0;
    int tag;

    N = 3; 
    int randBuff [N];



    MPI_Status status;
    MPI_Request request;

    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    genrate_matrix(randBuff, N);
    printf("\n");

    do {

        for (i = 0; i < N; i++){
            if (randBuff[0] == randBuff[i] && (i == N-1)){
                winFlag = 1;
                printf("process %d has won!", rank );
            }
        }

        int least_frequent; //index of the least frequent number
        int count = 0;
        int min = N;

        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                if (randBuff[j] == randBuff[i]) {
                    count++;
                }
            }
            if (count < min) {
                    min = count;
                    least_frequent = i;
                }
            count = 0;
        }
        selectedSending = randBuff[least_frequent];
        printf("picked %d\n from process %d\n",selectedSending, rank);
        selectedRecieving = randBuff[least_frequent];
        if (rank == N-1) {
            MPI_Isend(&selectedSending, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        }
        else {
            MPI_Isend(&selectedSending, 1, MPI_INT, (rank+1), 0, MPI_COMM_WORLD, &request);
        }
        MPI_Iprobe(rank-1, 0, MPI_COMM_WORLD, &tag, &status);
        MPI_Wait(&request, &status);
        if (rank == 0){
            MPI_Irecv(&selectedRecieving, 1, MPI_INT, N-1, 0, MPI_COMM_WORLD, &status);
        }
        else {
            MPI_Irecv(&selectedRecieving, 1, MPI_INT, (rank-1), 0, MPI_COMM_WORLD, &status);
        }
        MPI_Wait(&request, &status);

        for (i = 0; i < N; i++){
            printf("%d ", randBuff[i]);
        }
        printf("\n");

    }
    while (winFlag == 0);

    MPI_Finalize();

    return 0;
}