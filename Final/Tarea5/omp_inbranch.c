#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 10

int factorial(int n);

#pragma omp declare simd inbranch
float factor(int n){
	return 1.0/factorial(n);
}

int main(){

	float calculo;
	float e = 0.0;
	int i;

	#pragma omp simd private(calculo) reduction(+:e)
	for (i=0;i<N;++i){
		calculo = factor(i);
		e += calculo;
	}

	printf("El valor aproximado de e es %f\n",e);

	return 0;
}

int factorial(int n){
	int res = 1;
	int i;
	for (i=1;i<=n;++i){
		res *= i;
	}
	return res;
}