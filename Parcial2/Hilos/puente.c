#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define VACIO 0
#define IZQ 1
#define DER 1

pthread_mutex_t puente_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t izq_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t der_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t izq_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t der_cond = PTHREAD_COND_INITIALIZER;

int gente_cruzando = 0;
int gente_izq = 0;
int gente_der = 0;
int estado = VACIO;

void *cruzar_izquierda();
void *cruzar_derecha();

int main(){

	pthread_t izq_t;
	pthread_t der_t[2];

	int i;
	for (i = 0; i<2; ++i){
		pthread_create(der_t+i,NULL,cruzar_derecha,NULL);
	}
	pthread_create(&izq_t,NULL,cruzar_izquierda,NULL);
	sleep(2);
	pthread_t izq_f,der_f;
	pthread_create(&izq_f,NULL,cruzar_izquierda,NULL);
	pthread_create(&der_f,NULL,cruzar_derecha,NULL);

	pthread_join(der_t[0],NULL);
	pthread_join(der_t[1],NULL);
	pthread_join(izq_t,NULL);
	pthread_join(izq_f,NULL);
	pthread_join(der_f,NULL);



	return 0;
}

void* cruzar_izquierda(){
	pthread_mutex_lock(&izq_mutex);
	++gente_izq;
	pthread_mutex_unlock(&izq_mutex);
	printf("Cruzando hacia la izquierda\n");
	int entro = 0;
	while (!entro){
		pthread_mutex_lock(&puente_mutex);

		if (estado == DER){
			printf("Me toca esperar para cruzar a la izquierda\n");
			pthread_cond_wait(&izq_cond,&puente_mutex);
		}
			
		if (estado == VACIO){
			estado = IZQ;
			++gente_cruzando;
			entro = 1;
		}
			
		else if (estado == IZQ) {
			++gente_cruzando;
			entro = 1;
		}
		pthread_mutex_unlock(&puente_mutex);
	}
	printf("Estoy cruzando el puente hacia la izquierda\n");
	sleep(2);

	pthread_mutex_lock(&puente_mutex);
	--gente_cruzando;
	printf("Termine de cruzar hacia la izquierda\n");
	if (gente_cruzando == 0){
		estado = VACIO;
		printf("El puente quedo vacio\n");
		pthread_cond_broadcast(&der_cond);
	}
	pthread_mutex_unlock(&puente_mutex);
	pthread_exit(NULL);
}

void* cruzar_derecha(){
	pthread_mutex_lock(&der_mutex);
	++gente_der;
	pthread_mutex_unlock(&der_mutex);
	printf("Cruzando hacia la derecha\n");

	int entro = 0;
	while (!entro){
		pthread_mutex_lock(&puente_mutex);

		if (estado == IZQ){
			printf("Me toca esperar para cruzar a la derecha\n");
			pthread_cond_wait(&der_cond,&puente_mutex);
		}
			
		if (estado == VACIO){
			estado = DER;
			++gente_cruzando;
			entro = 1;
		}
			
		else if (estado == DER) {
			++gente_cruzando;
			entro = 1;
		}
		pthread_mutex_unlock(&puente_mutex);
	}
	printf("Estoy cruzando el puente hacia la derecha\n");
	sleep(2);

	pthread_mutex_lock(&puente_mutex);
	--gente_cruzando;
	printf("Termine de cruzar hacia la derecha\n");
	if (gente_cruzando == 0){
		estado = VACIO;
		printf("El puente quedo vacio\n");
		pthread_cond_broadcast(&izq_cond);
	}
	pthread_mutex_unlock(&puente_mutex);
	pthread_exit(NULL);
}