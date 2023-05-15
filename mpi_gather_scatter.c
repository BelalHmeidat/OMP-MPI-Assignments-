#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 10000

int main(int argc, char* argv[]){
    int size, rank, i;
    double *VA; //initial
    double *VAF; //final

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    double sum = 0;

    if (rank == 0){
        VA=(double*)malloc(N*N*sizeof(double));
         for (i=1; i<N*N; i++) {
            *(VA + i) =  rand() % 100;
        }
        VAF=(double*)malloc(N*N*sizeof(double));
    }
    
    int sub_NN = N*N/size; //size of the matrix for each proccess

    double *part; //sub matrix
    part=(double*)malloc(sub_NN*sizeof(double));

    clock_t begin = clock();

    MPI_Scatter(&VA[0], sub_NN, MPI_DOUBLE, &part[0], sub_NN, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // printf("Proceso %d data: \n",rank);
    for (i=0; i<sub_NN; i++) {
        sum+=*(part + i);
        // printf("%lf  \n",*(part + i));
    }
    // printf("\n\n"); 

    double totalSum;

    MPI_Allreduce(&sum, &totalSum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );

    float mean;

    mean = totalSum/(N*N);
    // printf("sum = %f mean = %f\n", sum, mean);

    for (i = 0; i < sub_NN; i++){
        part[i] /= mean;
    //     printf("%lf  \n",part[i]);
    }
    // printf("\n\n"); 


    MPI_Gather(&part[0], sub_NN, MPI_DOUBLE, &VAF[0], sub_NN, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    clock_t end = clock();


    if (rank == 0) {
        printf("original values:\n");
        for (i=0; i<N*N; i++) {
            printf("%f  \n",*(VA + i));
        }
        printf("\n\n"); 

        printf("Final values:\n");
        for (i=0; i<N*N; i++) {
            printf("%f  \n",*(VAF + i));
        }
        printf("\n\n"); 

    printf("mean = %lf", mean);
    printf("Time spent = %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    }





    return 0;

}