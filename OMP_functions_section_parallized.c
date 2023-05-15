#include <omp.h>
#include <stdio.h>

void sumaparcial(double *a, int n);
double sumatotal(double *a, int n, int THREADS);
double suma(double *a, int n, int THREADS);

void multparcial(double *a, int n);
double multtotal(double *a, int n, int THREADS);
double producto(double *a, int n, int THREADS);

int main(){
    int nthreads, tid;
    double vector[10],resultadosuma,resultadoproducto;
    int i,tam=10;
    omp_set_num_threads(4);
    srand(time(NULL));
    for (i=0; i<tam; i++)
        vector[i]=rand()%5 +1;
    #pragma omp parallel private (tid, i) shared(nthreads,vector, tam)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                //Qué thread soy?
                tid = omp_get_thread_num();
                //Cuántos threads hay?
                nthreads = omp_get_num_threads();
                
                printf("El thread %d, de %d, calcula la sección 1\n", tid, nthreads);
                resultadosuma=suma(vector,tam,nthreads);
                printf("El resultado de la suma es %f\n",resultadosuma);
            }
            #pragma omp section 
            {
                //Qué thread soy?
                tid = omp_get_thread_num();
                //Cuántos threads hay?
                nthreads = omp_get_num_threads();
                printf("El thread %d, de %d, calcula la sección 2\n", tid, nthreads);
                resultadoproducto=producto(vector,tam,nthreads);
                printf("El resultado del producto es %f\n",resultadoproducto);
            }
        }
    }

	
}

void sumaparcial(double *a, int n){
	int i;
	double s=0;
    // #pragma omp parallel for// private(i,s)
    // {
        for(i=0;i<n;i++)
            s+=a[i];
        a[0]=s;
    // }
}
double sumatotal(double *a, int n, int THREADS){
	int i;
	double s=0;
    // #pragma omp parallel for// private(i) shared(s)
    // {
	for(i=0;i<n;i+=n/THREADS)
		s+=a[i];
    // }
	return s;
}
double suma(double *a, int n, int THREADS){
	int i;
    #pragma omp parallel for private(i) shared(a, n, THREADS)
    // {
        for (i=0;i<THREADS;i++){
                sumaparcial(a, n);
                printf("Soy el thread %d i=%d\n", omp_get_thread_num(), i);	
        }
    // }
	return sumatotal(a, n, THREADS);
}



void multparcial(double *a, int n){
	int i;
	double s=1;
    // #pragma omp parallel for// private(i,s)
    // {
	    for(i=0;i<n;i++)
		    s*=a[i];
	    a[0]=s;
    // }
}
double multtotal(double *a, int n, int THREADS){
	int i;
	double s=1;
    // #pragma omp parallel for //private(i) shared(s)
    // {
        for(i=0;i<n;i+=n/THREADS)
            s*=a[i];
    // }
	return s;
}
double producto(double *a, int n, int THREADS){
	int i;
    #pragma omp parallel for //private (i) shared(a,n,THREADS)
    // {
        for (i=0;i<THREADS;i++){
            multparcial(a, n);
            printf("Soy el thread %d i = %d\n", omp_get_thread_num(), i);	
        }
    // }
	return multtotal(a, n, THREADS);
}

