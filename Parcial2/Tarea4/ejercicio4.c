#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SENSORES 4
#define CRITICO 35

pthread_mutex_t muestra_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t muestra_cond = PTHREAD_COND_INITIALIZER;

void *adquisicion_datos(void *arg);
void *sensor_central();

int muestra;

int main() {

	srand(time(NULL));
	pthread_t *sensores_t = (pthread_t*)malloc(SENSORES*sizeof(pthread_t));
	pthread_t central_t;

	pthread_create(&central_t,NULL,sensor_central,NULL);
	int i;
	for (i=0;i<SENSORES;++i){
		pthread_create(sensores_t+i,NULL,adquisicion_datos,i);
	}

	pthread_join(central_t,NULL);
	for (i=0;i<SENSORES;++i){
		pthread_join(*(sensores_t+i),NULL);
	}

	return 0;
}

void *adquisicion_datos(void *arg){
	int id = (int)arg;
	printf("------Iniciando el sensor %d-------\n",id);
	int random;
	while (1){
		random = rand() % 50;
		printf("Sensor %d midio el valor %d\n",id,random);
		if (random > CRITICO){
			printf("*****Sensor %d midio valor critico\n",id);
			pthread_mutex_lock(&muestra_mutex);
			muestra = random;
			pthread_cond_signal(&muestra_cond);
			pthread_mutex_unlock(&muestra_mutex);
		}
		sleep(1);
	}
	pthread_exit(NULL);
}

void *sensor_central(){

	printf("+++++++Iniciando consola central+++++++\n");

	while (1){
		pthread_mutex_lock(&muestra_mutex);
		pthread_cond_wait(&muestra_cond,&muestra_mutex);
		printf("Registre el valor %d\n",muestra);
		pthread_mutex_unlock(&muestra_mutex);

	}

	pthread_exit(NULL);
}