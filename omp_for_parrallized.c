#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define N 4
#define M 4

int main()
{
	int i,j, nthreads, tid, n, m, sum, a[M],c[N],b[M][N];
	
	srand(time(NULL));
	m=M;
	n=N;

	for (i=0; i<M; i++) 
	{
		for (j=0; j<N; j++) {
			b[i][j]=rand()%100;
		}		
	}
	for (i=0; i<N; i++) 
		c[i]=rand()%100;
	omp_set_num_threads(4);
	#pragma omp parallel for 
		for (i = 0; i < m; i++)
		{
			tid = omp_get_thread_num();
			nthreads = omp_get_num_threads();
			sum = 0;
			for (j = 0; j < n; j++)
				sum+=b[i][j]*c[j];
			a[i] = sum;
			printf("thread no. %d of %d threads is working on itteration no. %d\n", tid, nthreads, i);
		}
	for (i = 0; i < m; i++){
		for (j = 0; j < n; j++)
			printf("%d ", b[i][j]);
		printf("\n");
	}
	for (i = 0; i < m; i++)
		printf("%d\n",c[i]);
	for (i = 0; i < m; i++)
		printf("%d\n", a[i]);


		
	
	return 0;
}
