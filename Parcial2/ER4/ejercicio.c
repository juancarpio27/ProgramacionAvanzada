/*
Se compila: gcc –pthread

#pthread
int pthread_create(pthread_t *tid, const pthread_attr_t *tattr, void*(*start_routine)(void *), void *arg);
int pthread_exit(0);
int pthread_join(thread_t tid, void *status);

#Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//SI SON MUCHOS
pthread_mutex_init(&cubiertos[i],NULL);
int pthread_mutex_lock( pthread_mutex_t *mutex);
int pthread_mutex_unlock( pthread_mutex_t *mutex);


int pthread_mutex_trylock( pthread_mutex_t *mutex);

#condition
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_wait(&cond, &mutex);
pthread_cond_signal(&cond);


Ej:

pthread_mutex_lock(&mutex);
pthread_mutex_unlock(&mutex);
while (pthread_mutex_trylock(&mutex)){}

pthread_t * tid = (pthread_t *)malloc(nhilos * sizeof(pthread_t));
ó
pthread_t tid; <<si no necesitas el arreglo>>
pthread_create(tid+1, NULL, restar, (void *)1);
pthread_join(*(tid+i), NULL);
free(tid);
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mesa = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t papel_sem = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cerillos_sem = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tabaco_sem = PTHREAD_MUTEX_INITIALIZER;

void fumar(){
	printf("Estoy fumando\n");
	sleep(5);
	printf("Termine de fumar\n");
}

void colocar_materiales(){
	srand(time(NULL));
	int r = rand() % 3;
	if (r==0){
		pthread_mutex_lock(&cerillos_sem);
		pthread_mutex_lock(&tabaco_sem);
		printf("Estoy poniendo materiales");
		pthread_mutex_unlock(&cerillos_sem);
		pthread_mutex_unlock(&tabaco_sem);
	}
	else if (r==1){
		pthread_mutex_lock(&papel_sem);
		pthread_mutex_lock(&tabaco_sem);
		printf("Estoy poniendo materiales");
		pthread_mutex_unlock(&papel_sem);
		pthread_mutex_unlock(&tabaco_sem);
	}
	else {
		pthread_mutex_lock(&papel_sem);
		pthread_mutex_lock(&cerillos_sem);
		printf("Estoy poniendo materiales");
		pthread_mutex_unlock(&papel_sem);
		pthread_mutex_unlock(&cerillos_sem);
	}
}

void *agente(void *arg){

	while (1){
		pthread_mutex_lock(&mesa);
		colocar_materiales();
		pthread_mutex_unlock(&mesa);
	}

	pthread_exit(NULL);

}

void *fumador_papel(void *arg){

	while(1){
		pthread_mutex_lock(&mesa);
		pthread_mutex_lock(&cerillos_sem);
		while (pthread_mutex_trylock(&tabaco_sem)){
			pthread_mutex_unlock(&cerillos_sem);
			pthread_mutex_lock(&cerillos_sem);
		}
		fumar();
		pthread_mutex_unlock(&tabaco_sem);
		pthread_mutex_unlock(&cerillos_sem);
		pthread_mutex_unlock(&mesa);
	}

	pthread_exit(NULL);

}

void *fumador_tabaco(void *arg){

	while(1){
		pthread_mutex_lock(&mesa);
		pthread_mutex_lock(&cerillos_sem);
		while (pthread_mutex_trylock(&papel_sem)){
			pthread_mutex_unlock(&cerillos_sem);
			pthread_mutex_lock(&cerillos_sem);
		}
		fumar();
		pthread_mutex_unlock(&papel_sem);
		pthread_mutex_unlock(&cerillos_sem);
		pthread_mutex_unlock(&mesa);
	}

	pthread_exit(NULL);

}

void *fumador_cerillos(void *arg){

	while(1){
		pthread_mutex_lock(&mesa);
		pthread_mutex_lock(&papel_sem);
		while (pthread_mutex_trylock(&tabaco_sem)){
			pthread_mutex_unlock(&papel_sem);
			pthread_mutex_lock(&papel_sem);
		}
		fumar();
		pthread_mutex_unlock(&tabaco_sem);
		pthread_mutex_unlock(&papel_sem);
		pthread_mutex_unlock(&mesa);
	}

	pthread_exit(NULL);

}

int main()
{
	
	srand(time(NULL));

	pthread_t agente_tid;
	pthread_t fumador_papel_tid;
	pthread_t fumador_tabaco_tid;
	pthread_t fumador_cerillos_tid;

	pthread_create(&agente_tid,NULL,agente,NULL);
	pthread_create(&fumador_papel_tid,NULL,fumador_papel,0);
	pthread_create(&fumador_tabaco_tid,NULL,fumador_tabaco,1);
	pthread_create(&fumador_cerillos_tid,NULL,fumador_cerillos,2);


	pthread_join(agente_tid,NULL);
	pthread_join(fumador_papel_tid,NULL);
	pthread_join(fumador_cerillos_tid,NULL);
	pthread_join(fumador_tabaco_tid,NULL);

	return 0;
}