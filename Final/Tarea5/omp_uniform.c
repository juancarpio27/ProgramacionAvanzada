#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 10

#pragma omp declare simd uniform(m)
void *operacion(int *array, int *b, int m);

int main(){


	int *array = (int*)malloc(N*sizeof(int));
	int *b = (int*)malloc(N*sizeof(int));
	int i;

	for (i=0;i<N;++i){
		*(array+i) = i;
		*(b+i) = 2*i;
	}

	operacion(array,b,2);

	for (i=0;i<N;++i){
		printf("b[%d] = %d\n",i,*(b+i));
	}

	free(array);
	free(b);

	return 0;
}

void *operacion(int *array, int *b, int m){
	int i;
	for (i=0;i<N;++i){
		*(b+i) = *(array+i)/2;
	}
}