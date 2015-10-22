#include <stdio.h>
#include <pthread.h>

#define OPERACIONES 100000
#define CANTIDAD 3

int var = 0;

void* sumar();
void* restar();

int main(){

	pthread_t suma;
	pthread_t resta;

	pthread_create(&suma,NULL,sumar,NULL);
	pthread_create(&resta,NULL,restar,NULL);

	pthread_join(suma,NULL);
	pthread_join(resta,NULL);

	printf("El valor de var es: %d \n",var);

	return 0;
}

void* sumar(){

	int i;
	int temp;
	for (i=0;i<OPERACIONES;++i){

		temp = var;
		var += CANTIDAD;
	}
	pthread_exit(NULL);
}

void* restar(){

	int i;
	int temp;
	for (i=0;i<OPERACIONES;++i){

		temp = var;
		var -= CANTIDAD;
	}
	pthread_exit(NULL);
}