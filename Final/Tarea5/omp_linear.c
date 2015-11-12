#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 100

int main(){

	printf("++++++++linear++++++++\n");

	srand(time(NULL));

	int *array = (int*)malloc(sizeof(int));
	int i,m;
	for (i=0;i<N;++i){
		*(array+i) = rand() % 50;
	}

	m = 2;

	printf("Valor inicial m= %d con comportamiento linear\n",m);
	#pragma omp simd linear(m:4)
	for (i=0;i<N;i++){
		array[i] = array[i]*m;
	}

	for (i=0;i<N;i++){
		printf("array[%d] = %d\n",i,array[i]);
	}


	free(array);

	return 0;
}
