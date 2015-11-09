#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 10

int main(){

	printf("++++++++aligned++++++++\n");

	int *array = (int*)malloc(N*sizeof(int));
	int i;
	srand(time(NULL));

	/**El arreglo se encuentra alineado*/
	#pragma omp simd aligned(array)
	for (i=0;i<N;++i){
		*(array+i) = rand() % 50;
		printf("Valor %d del array alineado %d\n",i,*(array+i));
	}

	return 0;
}