#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define INTERFACES 8

typedef struct {
	int ip_origen;
	int ip_destino;
	int interfaz;
}

pthread_cond_t colas_cond[INTERFACES];
pthread_mutex_t colas_sem[INTERFACES];
pthread_mutex_t sem_totales = PTHREAD_MUTEX_INITIALIZER;

int paquetes_pendientes[INTERFACES];
int paquetes_totales = 0;

void *interfaz(void *arg);

int main() {
	srand(time(NULL));
	int i;
	pthread_t interfaces_t[INTERFACES];

	for (i=0;i<INTERFACES;i++){
		pthread_create(&interfaces_t[i],NULL,interfaz,i);
	}

	while (1){
		sleep(rand()%5);
		int paquete = rand() % INTERFACES;
		pthread_cond_signal(&colas_sem[paquete]);
	}

	for (i=0;i<INTERFACES;i++){
		pthread_join(interfaces_t[i],NULL);
	}

	return 0;
}

void *interfaz(void *arg){
	int id = (int)arg;
	printf("Soy la interfaz %d\n",id);
	int paquetes_internos = 0;

	while (1){
		pthread_mutex_lock(&colas_sem[id]);
		if (paquetes_pendientes[id] == 0){
			pthread_cond_wait(&colas_cond[id],&colas_sem[id]);
			--paquetes_pendientes[id];
		}
		pthread_mutex_unlock(&colas_sem[id]);

		paquetes_internos++;
		pthread_mutex_lock(&sem_totales);
		paquetes_totales++;
		pthread_mutex_unlock(&sem_totales);

	}

	pthread_exit(NULL);
}