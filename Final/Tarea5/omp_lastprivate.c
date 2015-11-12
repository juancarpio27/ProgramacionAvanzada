#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 20


int main(){

	printf("++++++++lastprivate++++++++\n");

	int valor = 0;
	int i;

	#pragma omp simd private(valor)
	for (i=0;i<N;++i){
		valor = i;
	}

	/**Como era private es privada en el ciclo y a la salida tiene su valor**/
	printf("Variable valor en private: %d\n",valor);

	valor = 0;

	#pragma omp simd lastprivate(valor)
	for (i=0;i<N;++i){
		valor = i;
	}

	/**Como es lastprivate entonces al salir del ciclo toma su ultimo valor**/
	printf("Variable valor con lastprivate: %d\n",valor);

	return 0;
}