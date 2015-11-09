#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 20

int factorial(int n);

int main(){

	float calculo;
	float e = 0.0;
	int i;

	#pragma omp simd private(calculo)
	for (i=0;i<N;++i){
		calculo = 1.0/factorial(i);
		e += calculo;
	}

	printf("El valor aproximado de e es %f\n",e);

	return 0;
}

int factorial(int n){
	int res = 1;
	int j;
	for (j=1;j<=n;j++){
		res *= j;
	}
	return res;
}