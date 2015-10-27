#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SILLAS 4 
#define ENANOS 7

pthread_mutex_t sillas_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t esperando_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_sentarse = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_sentado = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_comer = PTHREAD_COND_INITIALIZER;

int sillas_libres = 4;
int enanos_esperando = 0;

void *enano(void *arg);
void *blancanieves (void *arg);
void trabajar_mina();

int main(){
	srand(time(NULL));
	int i;
	pthread_t enanos_t[ENANOS];
	pthread_t blancanieves_t;
	pthread_create(&blancanieves_t,NULL,blancanieves,NULL);
	for (i=0;i<ENANOS;++i){
		pthread_create(&enanos_t[i],NULL,enano,i);
	}


	for (i=0;i<ENANOS;++i){
		pthread_join(enanos_t[i],NULL);
	}
	pthread_join(blancanieves_t,NULL);
	return 0;
}

void *enano(void *arg){
	int id = (int)arg;

	while (1){
		trabajar_mina(id);
		printf("Enano %d: me quiero sentar en la silla\n",id);
		int sentado = 0;
		
		while (!sentado){
			pthread_mutex_lock(&sillas_mutex);
			if (sillas_libres > 0){
				sillas_libres--;
				sentado = 1;
				printf("Enano %d: Ya me sente\n",id);
				pthread_mutex_lock(&esperando_mutex);
				++enanos_esperando;
				pthread_mutex_unlock(&esperando_mutex);
			}
			else {
				printf("Enano %d: me toca esperar a que se libere una silla\n",id);
				pthread_cond_wait(&cond_sentarse,&sillas_mutex);
			}
		}
		pthread_mutex_unlock(&sillas_mutex);

		pthread_cond_signal(&cond_sentado);

		pthread_mutex_lock(&esperando_mutex);
		pthread_cond_wait(&cond_comer,&esperando_mutex);
	
		pthread_mutex_unlock(&esperando_mutex);

		printf("Enano %d: ya estoy comiendo,  a trabajar de nuevo\n",id);
		sleep(4);

		pthread_mutex_lock(&sillas_mutex);
		printf("Enano %d: me pare de la silla\n",id);
		sillas_libres++;
		pthread_mutex_unlock(&sillas_mutex);
	}

	pthread_exit(NULL);
}

void *blancanieves(void *arg){
	while (1){
		pthread_mutex_lock(&sillas_mutex);
		if (sillas_libres == 0){
			printf("Me voy a dar un paseo\n");
			pthread_cond_wait(&cond_sentado,&sillas_mutex);
		}
		pthread_mutex_unlock(&sillas_mutex);
		
		pthread_mutex_lock(&esperando_mutex);
		if (enanos_esperando > 0){
			printf("Voy a cocinar para un enanito\n");
			pthread_cond_signal(&cond_comer);
			--enanos_esperando;
		}
		pthread_mutex_unlock(&esperando_mutex);
	}
	
	pthread_exit(NULL);
}

void trabajar_mina(int i){
	int tiempo_trabajo = rand() % 10;
	printf("Yohooo yohooo ---- Soy el enano %d y voy a trabajar %d ------ Yohoooo yohooooo\n",i,tiempo_trabajo);
	sleep(tiempo_trabajo);
}