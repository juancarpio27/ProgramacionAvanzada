#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 10

#pragma omp declare simd uniform(m)
void *operacion(int *array, int *b, int m);

int main(){

	printf("++++++++uniform++++++++\n");

	int *array = (int*)malloc(N*sizeof(int));
	int *b = (int*)malloc(N*sizeof(int));
	int i;

	for (i=0;i<N;++i){
		*(array+i) = i;
		*(b+i) = 2*i;
	}

	int m = 2;
	printf("Llamando a funcion, m=%d tendra valor uniforme para la funcion\n",m);
	operacion(array,b,m);

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
		*(b+i) = *(array+i)/m;
	}
}