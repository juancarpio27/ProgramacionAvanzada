#include <stdio.h>
#include <pthread.h>

#define OPERACIONES 100000
#define CANTIDAD 3

int var = 0;
int var2 = OPERACIONES;

pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;

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
	printf("El valor de resta es: %d \n",var2);

	return 0;
}

void* sumar(){

	int i;
	int temp;
	for (i=0;i<OPERACIONES;++i){

		/*REGION CRITICA*/
		pthread_mutex_lock(&mutex_1); //DOWN
		temp = var;
		var += CANTIDAD;

		pthread_mutex_lock(&mutex_2);
		var2 = var2 + 1;
		pthread_mutex_unlock(&mutex_2);

		pthread_mutex_unlock(&mutex_1); //UP
		/*REGION CRITICA*/
	}
	pthread_exit(NULL);
}

void* restar(){

	int i;
	int temp;
	for (i=0;i<OPERACIONES;++i){

		/*REGION CRITICA*/
		pthread_mutex_lock(&mutex_1); //DOWN
		temp = var;
		var -= CANTIDAD;

		pthread_mutex_lock(&mutex_2);
		var2 = var2 - 1;
		pthread_mutex_unlock(&mutex_2);

		pthread_mutex_unlock(&mutex_1); //UP
		/*REGION CRITICA*/

	}
	pthread_exit(NULL);
}