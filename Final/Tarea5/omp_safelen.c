#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 10

int main(){

	printf("++++++++safelen++++++++\n");

	int c[N];
	int i;

	c[0] = 1;
	c[1] = 2;

	/**Se tiene el doble del que se encuentra dos posiciones atras. 
	Por el safelen(2) no permite que se ejecuten operaciones separadas
	por dos ciclos de distancia. Se "vectoriza" de 2 en 2**/
	#pragma omp simd safelen(2)
	for (i=2;i<N;++i){
		c[i] = c[i-2]*2;
	}

	for (i=0;i<N;++i){
		printf("c[%d] = %d\n",i,c[i]);
	}

	return 0;
}