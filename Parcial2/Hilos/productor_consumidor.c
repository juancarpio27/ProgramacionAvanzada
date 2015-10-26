#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10
#define N 100

int elementos[SIZE] 

int in = 0;
int out = 0;

int total = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER
pthread_cond_t consume_t = PTHREAD_COND_INITIALIZER
pthread_cond_t produce_t = PTHREAD_COND_INITIALIZER

void *productor(void *);
void *consumidor(void *);

int main(){

	pthread_t produce,consume;

	produce = pthread_create(&produce,NULL,productor,NULL);
	consume = pthread_create(&consume,NULL,consumidor,NULL);

	pthread_join(produce,NULL);
	pthread_join(consume,NULL);

	return 0;
}

void *productor(void *arg){

	int i = 0;
	while (i < N){

		pthread_mutex_lock(&mutex);

		if (total < SIZE){

		elementos[in] = i;
		++i;
		in += i % SIZE;
		++total;
		if (total == 1){
			pthread_cond_signal(&consume_t);
		}
		}
		else {
			pthread_cond_wait(&produce_t,&mutex);
		}

		pthread_mutex_unlock(&mutex);
	}
}

void  *consumidor(void *arg){
	int i = 0;
	while (i< N){

		usleep(200);

		pthread_mutex_lock(&mutex);

		if (total>0){
			printf("Se consumio el elemento %d \n ",elementos[in] = i);
			++i;
			out += i % SIZE;
			--total;
			if (total == SIZE - 1){
				pthread_cond_signal(&produce_t);
			}
		}
		else{
			pthread_cond_wait(&consume_t,&mutex);
		}

		pthread_mutex_unlock(&mutex);
	}
}