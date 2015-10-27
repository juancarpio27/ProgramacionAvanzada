#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define SENSORES 4
#define CRITICO 45

pthread_mutex_t *muestra_mutex;
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t muestra_cond = PTHREAD_COND_INITIALIZER;


void *adquisicion_datos(void *arg);
void *sensor_central();
void recibir_critico(int id);
void registrar_valores(int id);
void cerrar_archivo(int id);

pthread_t *sensores_t;
pthread_t central_t;

int *muestra;
int *lectura;

FILE *f;

int main() {

	muestra_mutex = (pthread_mutex_t*)malloc(SENSORES*sizeof(pthread_mutex_t));
	muestra = (int*)malloc(SENSORES*sizeof(int));
	lectura = (int*)malloc(SENSORES*sizeof(int));

	srand(time(NULL));
	f = fopen("file.txt", "w+");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	signal(SIGINT,cerrar_archivo);

	int i;
	for (i=0;i<SENSORES;++i){
		pthread_mutex_init(muestra_mutex+i,NULL);
	}

	sensores_t = (pthread_t*)malloc(SENSORES*sizeof(pthread_t));

	pthread_create(&central_t,NULL,sensor_central,NULL);

	for (i=0;i<SENSORES;++i){
		*(lectura+i) = 0;
		pthread_create(sensores_t+i,NULL,adquisicion_datos,i);
	}

	pthread_join(central_t,NULL);
	for (i=0;i<SENSORES;++i){
		pthread_join(*(sensores_t+i),NULL);
	}

	return 0;
}

void recibir_critico(int id){

	int i = 0;
	while (i<SENSORES){
		pthread_mutex_lock(&muestra_mutex[i]);
		if (*(lectura+i)){
			pthread_mutex_lock(&file_mutex);
			fprintf(f,"Central: Recibi valor critico %d de %d\n",*(muestra+i),i);
			pthread_mutex_unlock(&file_mutex);
			*(lectura+i) = 0;
		}
		pthread_mutex_unlock(&muestra_mutex[i]);
		++i;
	}
}

void registrar_valores(int id){
	int i;
	for (i=0;i<SENSORES;++i){
		pthread_mutex_lock(muestra_mutex+i);
		fprintf(f,"Lectura del sensor %d: %d\n",i,*(muestra+i));
		pthread_mutex_unlock(muestra_mutex+i);
	}
	int medir = rand() % 20;
	alarm(medir);
}

void cerrar_archivo(int id){
	fclose(f);
	free(muestra_mutex);
	free(muestra);
	free(lectura);
	raise(SIGTERM);
}

void *adquisicion_datos(void *arg){
	int id = (int)arg;
	printf("------Iniciando el sensor %d-------\n",id);
	int random;
	while (1){
		random = rand() % 50;
		pthread_mutex_lock(muestra_mutex+id);
		*(muestra+id) = random;
		pthread_mutex_unlock(muestra_mutex+id);
		if (random > CRITICO){
			pthread_mutex_lock(muestra_mutex+id);
			*(lectura+id) = 1;
			pthread_kill(central_t,SIGUSR1);
			pthread_mutex_unlock(muestra_mutex+id);
		}
		int s = sleep(1);
	}
	pthread_exit(NULL);
}

void *sensor_central(){

	printf("+++++++Iniciando consola central+++++++\n");
	signal(SIGUSR1,recibir_critico);
	signal(SIGALRM,registrar_valores);
	int medir = rand() % 10;
	alarm(medir);
	
	while (1){

	}

	pthread_exit(NULL);
}