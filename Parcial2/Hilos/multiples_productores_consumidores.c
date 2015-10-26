
#include SIZE 10
#define N 100

#define PRODUCTORES 5
#define CONSUMIDORES 3

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

	pthread_t produce[PRODUCTORES],consume[CONSUMIDORES];

	int i;
	for (i=0;i<PRODUCTORES;++i){
		pthread_create(produce+i,NULL,productor,NULL);
	}

	for (i=0;i<CONSUMIDORES;++i){
		pthread_create(consume+i,NULL,productor,NULL);
	}
	
	for (i=0;i<PRODUCTORES;++i){
		pthread_join(*(produce+i),NULL,productor,NULL);
	}

	for (i=0;i<CONSUMIDORES;++i){
		pthread_join(*(consume+i),NULL,productor,NULL);
	}



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