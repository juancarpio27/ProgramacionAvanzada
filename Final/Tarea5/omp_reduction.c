#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 20

int factorial(int n);

int main(){

	int suma = 0;
	int i;

	printf("++++++++reduction++++++++\n");

	srand(time(NULL));

	int array[N];
	for (i=0;i<N;++i){
		array[i] = rand() % 50;
	}

	#pragma omp simd reduction(+:suma)
	for (i=0;i<N;++i){
		suma += array[i];
	}

	printf("El promedio del arreglo es %d\n",suma/N);

	return 0;
}

