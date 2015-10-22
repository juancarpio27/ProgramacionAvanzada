#include <stdio.h>
#include <pthread.h>

#define N 2
#define HILOS 5

void *suma(void *);

int main(){

	pthread_t * tids = (pthread_t*)malloc(HILOS*sizeof(pthread_t));

	int i;
	for (i =0; i<HILOS;++i){
		printf("Creando el hilo  %d++++++\n",i);
		pthread_create((tids+i),NULL,suma,i);
	}

	/*Procesado en el hilo principal*/
	/*int i;
	for (i=0;i<N;++i){
		printf("M - Soy el padre en la iteracion %d\n",i);
	}*/

	//Esperar a que todos los hilos ermines

	for (i=0;i<N;++i){
		pthread_join(*(tids+i),NULL);
	}
	pthread_exit(NULL);
	return 0;
}

void *suma(void *arg){
	//int i;
	/*for (i=0;i<N;++i){
		printf("H - Soy el Hilo 1 en la iteracion %d\n",i);
	}*/
	printf("H --- Soy el hilo %d\n",(int)arg);
	pthread_exit(NULL);
}