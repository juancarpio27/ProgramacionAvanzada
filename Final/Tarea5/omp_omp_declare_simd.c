#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 1000000

void generar(int *array);

#pragma omp declare simd
int *calcular_maximos(int *a, int *b);

int max(int a, int b);

int main(){

	printf("++++++++omp declare simd++++++++\n");
	int i;

	int *a = (int*)malloc(N*sizeof(int));
	int *b = (int*)malloc(N*sizeof(int));
	

	generar(a);
	generar(b);

	/**Calcular maximo con una sola instruccion para multiples datos**/
	int *c = calcular_maximos(a,b);

	printf("Arreglo C contiene el maximo entre cada arreglo\n");

	return 0;
}

/**Generar los arreglos**/
void generar(int *array){
	int i;
	for (i=0;i<N;++i){
		*(array+i) = rand() % 100;
	}
}

/**Maximo entre dos numeros**/
int max(int a,int b){
	if (a>b)
		return a;
	else
		return b;
}

int *calcular_maximos(int *a, int *b){
	int *array = (int*)malloc(N*sizeof(int));
	int i;
	for (i=0;i<N;++i){
		*(array+i) = max(*(a+i),*(b+i));
	}
	return array;
}