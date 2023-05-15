#include <stdio.h>
#include <omp.h>

int main () {
	int nthreads, tid;
	#pragma omp parallel private (tid)
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				nthreads=omp_get_num_threads();
				tid=omp_get_thread_num();
				printf("Thread %d of %d sec1\n", tid, nthreads);
			}	
			#pragma omp section
			{		
				tid=omp_get_thread_num();
				nthreads=omp_get_num_threads();
				printf("Thread %d of %d sec2\n", tid, nthreads);
			}	
			#pragma omp section
			{		
				tid=omp_get_thread_num();
				nthreads=omp_get_num_threads();
				printf("Thread %d of %d sec3\n", tid, nthreads);
			}	
			#pragma omp section
			{		
				tid=omp_get_thread_num();
				nthreads=omp_get_num_threads();
				printf("Thread %d of %d sec4\n", tid, nthreads);
			}		
		}	
	}
	return 0;
}
