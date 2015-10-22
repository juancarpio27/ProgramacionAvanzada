#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

#define FILOSOFOS 5
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cubiertos[FILOSOFOS];

void pensar(int i);
void comer(int i);
void filosofo(void *arg);

int main(){

	srand(time(NULL));
	pthread_t * tids = (pthread_t*)malloc(FILOSOFOS*sizeof(pthread_t));
	int i;

	for (i=0;i<FILOSOFOS;++i){
		pthread_mutex_init(&cubiertos[i],NULL);
	}

	for (i =0; i<FILOSOFOS;++i){
		pthread_create((tids+i),NULL,filosofo,i);
	}

	for (i=0;i<FILOSOFOS;++i){
		pthread_join(*(tids+i),NULL);
	}

	return 0;

}

void pensar(int i){
	int r = rand() %  15;
	printf("Soy el filosofo %d pensando!\n",i);
	sleep(r);
}

void comer(int i){
	int r = rand() % 10;
	printf("Soy el filosofo %d comiendo!\n",i);
	sleep(r);
}

void filosofo(void *arg){
	int i = (int)arg;
	printf("Soy el filosofo %d\n",i);
	while (1){
		pensar(i);

		pthread_mutex_lock(&cubiertos[i]); 
		while (pthread_mutex_lock(&cubiertos[(i+1)%5])) {
            pthread_mutex_unlock(&cubiertos[i]);
            pthread_mutex_lock(&cubiertos[i]);
        }
		comer(i);
		pthread_mutex_unlock(&cubiertos[i]); 
		pthread_mutex_unlock(&cubiertos[(i+1)%5]); 
		
	}
	pthread_exit(NULL);
}