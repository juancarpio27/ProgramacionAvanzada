#include <omp.h>
#include <stdio.h>

#define N 10000000

int main(){


	int tid;
	int i;
	int suma = 0;

	#pragma omp parallel private(tid)
	{
		
		tid = omp_get_thread_num();

		#pragma omp for private(i) reduction(+:suma)
			for (i=0;i<N;++i){
				#pragma omp atomic
				suma += 1;
			}
	}

	printf("Suma %d\n",suma);

	return 0;
}