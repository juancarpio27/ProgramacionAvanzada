#include <pthread.h>
#include <time.h>
#include <stdio.h>

#define ELEMENTOS 10000000
#define REGLONES 4

int suma = 0;
int *numeros;
int longitud = ELEMENTOS / REGLONES;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *sumar_reglon(void *arg);

int main(){

	numeros = (int*)malloc(ELEMENTOS*sizeof(int));
	srand(time(NULL));

	int i;

	pthread_t hilos[REGLONES];

	FILE *f;
	int suma_2 = 0;

    f = fopen("prueba.txt", "w");

	//GENERAR A LOS ALEATORIOS
	fprintf(f,"%d\n",ELEMENTOS);
	for (i=0;i<ELEMENTOS;++i){
		int r = rand() % 20;
		fprintf(f,"%d\n",r);
		suma_2 += r;
	}

	FILE *read_f;
	read_f = fopen("prueba.txt", "r");

	int c;
	fscanf(read_f,"%d",&c);

	for (i=0;i<ELEMENTOS;++i){
		fscanf(read_f,"%d",&c);
		*(numeros+i)=c;
	}


	clock_t tic = clock();

	for (i=0;i<REGLONES;++i){
		int intervalo = longitud * i;
		pthread_create(&hilos[i],NULL,sumar_reglon,intervalo);
	}

	for (i=0;i<REGLONES;++i){
		pthread_join(hilos[i],NULL);
	}

	clock_t toc = clock();

	printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

	printf("La suma de los numeros es %d\n",suma);
	printf("La suma deberia ser: %d\n",suma_2);

	return 0;
}

void *sumar_reglon(void *arg){
	int rango = (int)arg;
	int local = 0;
	int j;
	for (j=rango;j<rango+longitud;++j){
		local = local + *(numeros+j);
	}

	pthread_mutex_lock(&mutex);
	suma += local;
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}