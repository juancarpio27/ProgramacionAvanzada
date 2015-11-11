#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 1000

int main(){

	printf("++++++++linear++++++++\n");
	srand(time(NULL));

	int *array = (int*)malloc(sizeof(int));
	int i,m;
	for (i=0;i<N;++i){
		*(array+i) = rand() % 50;
	}

	m = 2;
	#pragma omp simd linear(m:4)
	for (i=0;i<N;i++){
		array[i] = array[i]*m;
	}

	free(array);

	return 0;
}
