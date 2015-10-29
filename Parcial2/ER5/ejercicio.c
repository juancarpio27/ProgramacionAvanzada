#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define COMPLEJOS 2
#define SALAS 3
#define SILLAS 10

#define TAQUILLAS 2

#define OCUPADA 1
#define VACIA 0

//Un mutex por cada silla que se desee reservar
pthread_mutex_t *sillas_mutex;
int *estado_sillas;

//Un mutex por cada taquilla del cine
pthread_mutex_t *taquillas_mutex;

pthread_mutex_t taquillas_mutex_fijo = PTHREAD_MUTEX_INITIALIZER;

int taquillas_libres = TAQUILLAS;
pthread_cond_t taquillas_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t sillas_cond = PTHREAD_COND_INITIALIZER;

void *comprar_taquilla(void *arg);
void *comprar_aplicacion(void *arg);
void *comprar_pagina(void *arg);

int main(){

	int i;
	//Total de sillas que existen en el cine
	int total_asientos = COMPLEJOS*SALAS*SILLAS;
	int total_taquillas = COMPLEJOS*TAQUILLAS;

	sillas_mutex = (pthread_mutex_t*)malloc(total_asientos*sizeof(pthread_mutex_t));
	estado_sillas = (int*)malloc(total_asientos*sizeof(int));
	taquillas_mutex = (pthread_mutex_t*)malloc(total_taquillas*sizeof(pthread_mutex_t));

	for (i=0;i<total_asientos;++i){
		pthread_mutex_init(sillas_mutex+i,NULL);
		*(estado_sillas+i) = VACIA;
	}

	for (i=0;i<total_taquillas;++i){
		pthread_mutex_init(taquillas_mutex+i,NULL);
	}


	pthread_t *compradores = (pthread_t*)malloc(10*sizeof(pthread_t));
	for (i=0; i < 10; i++){
		int r = rand() % 3;
		if (r == 0)
			pthread_create(compradores+i,NULL,comprar_aplicacion,i);
		else if (r == 1)
			pthread_create(compradores+i,NULL,comprar_pagina,i);
		else
			pthread_create(compradores+i,NULL,comprar_taquilla,i);
	}

	for (i=0; i < 10; i++){
		pthread_join(*(compradores+i),NULL);
	}

	free(sillas_mutex);
	free(estado_sillas);
	free(taquillas_mutex);
	free(compradores);

	return 0;
}

void *comprar_aplicacion(void *arg){
	int id = (int)arg;
	int compro = 0;
	printf("Cliente %d: Voy a comprar en aplicacion\n",id);
	int silla_deseada;

	while (!compro){

		int complejo = rand() % COMPLEJOS;
		int sala = rand() % SALAS;
		int silla = rand() % SILLAS;
		printf("El cliente %d va al complejo %d, sala %d, silla %d\n",id,complejo,sala,silla);

		//Bloquear la silla que es escogio 
		/*Es como si fuese una matriz, me voy al complejo que quiero, luego a su sala y luego a su silla*/
		silla_deseada = (complejo*COMPLEJOS+sala*SALAS+silla);
		pthread_mutex_lock(sillas_mutex+silla_deseada);
		if (*(estado_sillas+silla_deseada) == 0){
			printf("La silla esta libre la puedo comprar :)\n");
			int decision = rand() % 2;
			if (decision == 0){
				printf("Cliente %d: No la voy a comprar pq soy un aburrido\n",id);
				pthread_exit(NULL);
			}

			if (*(estado_sillas+silla_deseada) == OCUPADA){
				pthread_cond_wait(&sillas_cond,sillas_mutex+silla_deseada);
			}

			if (decision == 1) {
				printf("Comprando: cliente %d va al complejo %d, sala %d, silla %d\n",id,complejo,sala,silla);
				*(estado_sillas+silla_deseada) = OCUPADA;
				compro = 1;
			}
		}
		pthread_mutex_unlock(sillas_mutex+silla_deseada);
	}

	//Ver la pelicula
	sleep(5);

	pthread_mutex_lock(sillas_mutex+silla_deseada);
	*(estado_sillas+silla_deseada) = VACIA;
	pthread_mutex_unlock(sillas_mutex+silla_deseada);

	pthread_exit(NULL);

}

void *comprar_pagina(void *arg){
	int id = (int)arg;
	int compro = 0;
	printf("Cliente %d: Voy a comprar en web\n",id);

	int silla_deseada;

	while (!compro){

		int complejo = rand() % COMPLEJOS;
		int sala = rand() % SALAS;
		int silla = rand() % SILLAS;
		printf("El cliente %d va al complejo %d, sala %d, silla %d\n",id,complejo,sala,silla);

		//Bloquear la silla que es escogio 
		/*Es como si fuese una matriz, me voy al complejo que quiero, luego a su sala y luego a su silla*/
		silla_deseada = (complejo*COMPLEJOS+sala*SALAS+silla);
		pthread_mutex_lock(sillas_mutex+silla_deseada);
		if (*(estado_sillas+silla_deseada) == 0){
			printf("La silla esta libre la puedo comprar :)\n");
			int decision = rand() % 2;
			if (decision == 0){
				printf("Cliente %d: No la voy a comprar pq soy un aburrido\n",id);
				pthread_exit(NULL);
			}

			if (*(estado_sillas+silla_deseada) == OCUPADA){
				pthread_cond_wait(&sillas_cond,sillas_mutex+silla_deseada);
			}

			if (decision == 1) {
				printf("Comprando: cliente %d va al complejo %d, sala %d, silla %d\n",id,complejo,sala,silla);
				*(estado_sillas+silla_deseada) = OCUPADA;
				compro = 1;
			}
		}
		pthread_mutex_unlock(sillas_mutex+silla_deseada);
	}

	//Ver la pelicula
	sleep(5);

	pthread_mutex_lock(sillas_mutex+silla_deseada);
	*(estado_sillas+silla_deseada) = VACIA;
	pthread_cond_broadcast(&sillas_cond);
	pthread_mutex_unlock(sillas_mutex+silla_deseada);

	pthread_exit(NULL);
}

void *comprar_taquilla(void *arg){

	int id = (int)arg;
	printf("Cliente %d: Voy a comprar en taquilla\n",id);

	pthread_mutex_lock(&taquillas_mutex_fijo);
	int entre = 0;
	while (!entre){
		if (taquillas_libres == 0){
			pthread_cond_wait(&taquillas_cond,&taquillas_mutex_fijo);
		}
		else {
			--taquillas_libres;
			printf("Ya tengo mi taquilla\n");
			entre = 1;
		}
	}
	pthread_mutex_unlock(&taquillas_mutex_fijo);

	printf("Ya en la taquilla voy a comprar\n");
	sleep(5);

	int silla_deseada;
	int compro;

	while (!compro){

		int complejo = rand() % COMPLEJOS;
		int sala = rand() % SALAS;
		int silla = rand() % SILLAS;
		printf("El cliente %d va al complejo %d, sala %d, silla %d\n",id,complejo,sala,silla);

		//Bloquear la silla que es escogio 
		/*Es como si fuese una matriz, me voy al complejo que quiero, luego a su sala y luego a su silla*/
		silla_deseada = (complejo*COMPLEJOS+sala*SALAS+silla);
		pthread_mutex_lock(sillas_mutex+silla_deseada);
		if (*(estado_sillas+silla_deseada) == 0){
			printf("La silla esta libre la puedo comprar :)\n");
			int decision = rand() % 2;
			if (decision == 0){
				printf("Cliente %d: No la voy a comprar pq soy un aburrido\n",id);
				pthread_exit(NULL);
			}

			if (*(estado_sillas+silla_deseada) == OCUPADA){
				pthread_cond_wait(&sillas_cond,sillas_mutex+silla_deseada);
			}

			if (decision == 1) {
				printf("Comprando: cliente %d va al complejo %d, sala %d, silla %d\n",id,complejo,sala,silla);
				*(estado_sillas+silla_deseada) = OCUPADA;
				compro = 1;
			}
		}
		pthread_mutex_unlock(sillas_mutex+silla_deseada);
	}

	/**Ya compre ahora libero la silla*/
	pthread_mutex_lock(&taquillas_mutex_fijo);
	++taquillas_libres;
	pthread_cond_broadcast(&taquillas_cond);
	pthread_mutex_unlock(&taquillas_mutex_fijo);


	//Ver la pelicula
	sleep(5);

	pthread_mutex_lock(sillas_mutex+silla_deseada);
	*(estado_sillas+silla_deseada) = VACIA;
	pthread_cond_broadcast(&sillas_cond);
	pthread_mutex_unlock(sillas_mutex+silla_deseada);


	pthread_exit(NULL);
}