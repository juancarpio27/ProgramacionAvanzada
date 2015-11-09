#include <omp.h>
#include <stdio.h>

#define N 100000000

int main(){


	int tid;
	int i;
	int suma = 0;

	double start = omp_get_wtime();

	#pragma omp parallel private(tid,i) reduction(+:suma)
	{
		#pragma omp for schedule(dynamic,100)
		for (i=0;i<N;++i){
			//#pragma omp atomic
			suma += i;
		}
	}

	double finish = omp_get_wtime();

	printf("Suma total: %d / tiempo %f\n",suma,finish-start);

	return 0;
}