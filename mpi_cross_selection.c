#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>


int main(int argc, char **argv) {
  
  
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    MPI_Status stat;

    clock_t begin, end; 

    int i, j, k, t;

    int N = 10;

    int matrix [N][N]; //whole matrix
    int total_elements; //number of elements in the cross only

    int recievedSum [size - 1]; //sum from all processes except 0
    int sum[1] = {0}; //sum from each process except 0


    if (rank == 0) {

        for (int e = 0; e < N; e++) {
            for (int f = 0; f < N; f++) {
                matrix[e][f] = rand() % 10;
                printf("%d ", matrix[e][f]);
            }
            printf("\n");
        }
        begin = clock();
            
        i = rand() % N;
        j = rand() % N;

        k = rand() % N;
        t = rand() % N;

        for (; i > j;){
            i = rand() % N;
        }

        for (; k > t;){
            k = rand() % N;
        }
    
    }
    MPI_Datatype newType; //cross

    int count; //count of the blocks
    count = k + N - t;
    int e; //index pointer

    int blockLength [k + 1 + (N - t)];
    int displacements [k + 1 + (N - t)];

    //fist area displacments and lengths
    for (e = 0; e < k; e++) {
        displacements [e] = i + e * N;
        // printf("_%d ", displacements [e]);
        blockLength [e] = j - i + 1;
        // printf("%d ", blockLength [e]);
    }
    // printf("\n");
    //middle area displacments
    displacements[k] = k * N;
    // printf("__%d ", displacements[k]);
    //middlea area lengths
    blockLength [k] = N * ( t - k + 1); 
    // printf("%d ", blockLength [k]);
    // printf("\n");
    e = 0;
    //bottom area displacments and legnths
    for (; e < N - t -1; e++) {
        displacements [k + e + 1] = displacements[k] + blockLength[k] + i  + e * N;
        // printf("___%d ", displacements [k + e + 1]);
        blockLength [k + e + 1] = j - i + 1;
        // printf("%d ", blockLength [k + e + 1]);

    }

    MPI_Type_indexed(count , &blockLength , &displacements , MPI_INT , &newType);

    MPI_Type_commit(&newType);

    total_elements = (j - i + 1) * (t - k + 1);
    int recievedCross [total_elements];

    

    if (rank == 0){
        MPI_Bcast( &matrix , 1 , newType , 0 , MPI_COMM_WORLD);
    }
    else{
        MPI_Recv(&recievedCross, total_elements, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat); 
        for (int z = 0; z < total_elements; z++) {
            printf("%d ", recievedCross[z]);
            printf("\n");
            sum[0]+= recievedCross[z];
        }
        printf("Sum from process %d is %d\n", rank, sum[0]);
        MPI_Send( &sum , 1 , MPI_INT , 0 , 0 , MPI_COMM_WORLD);
        

        
    }
    
    // MPI_Recv(&recievedSum, size - 1, MPI_INT, rank, 0, MPI_COMM_WORLD, &stat);
    MPI_Gather( &sum , 1 ,MPI_INT , &recievedSum , size -1  , MPI_INT , 0 , MPI_COMM_WORLD);
    end = clock();
    if (rank == 0) {
        for (int z = 0; z < size - 1; z++) {
            printf("%d ", recievedSum[z]);
            printf("\n");
        }
    }
    printf("Time: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);

    MPI_Finalize();
    
    return 0;

}


