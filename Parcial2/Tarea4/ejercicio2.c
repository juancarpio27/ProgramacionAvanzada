#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define VACIO 0
#define MUJERES 1
#define HOMBRES 2

pthread_mutex_t bano_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t hombre_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mujer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hombre_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t mujer_cond = PTHREAD_COND_INITIALIZER;

int personas_dentro = 0;
int espera_mujeres = 0;
int espera_hombres = 0;
int estado = VACIO;

void mujer_quiere_entrar();
void hombre_quiere_entrar();
void mujer_sale();
void hombre_sale();

void *mujer();
void *hombre();


int main(){

	printf("Sanitario vacio\n");
	pthread_t hombres;
	pthread_t mujeres[2];

	int i;
	for (i = 0; i<2; ++i){
		pthread_create(mujeres+i,NULL,mujer,NULL);
	}
	pthread_create(&hombres,NULL,hombre,NULL);
	sleep(2);
	pthread_t hombre_f,mujer_f;
	pthread_create(&mujer_f,NULL,mujer,NULL);
	pthread_create(&hombre_f,NULL,hombre,NULL);

	pthread_join(mujeres[0],NULL);
	pthread_join(mujeres[1],NULL);
	pthread_join(hombres,NULL);
	pthread_join(hombre_f,NULL);
	pthread_join(mujer_f,NULL);


	return 0;
}

void *mujer(){
	
	mujer_quiere_entrar();
	sleep(3);
	mujer_sale();
	pthread_exit(NULL);
}

void mujer_quiere_entrar(){

	pthread_mutex_lock(&mujer_mutex);
	printf("Llega una mujer (%d en espera)\n",++espera_mujeres);
	pthread_mutex_unlock(&mujer_mutex);
	int listo = 0;

	while (!listo){

	pthread_mutex_lock(&bano_mutex);

		if (estado == HOMBRES){
			pthread_cond_wait(&mujer_cond,&bano_mutex);
		}
			
		if (estado == VACIO){
			estado = MUJERES;
			printf("Entra una mujer (%d en espera)\n",--espera_mujeres);
			++personas_dentro;
			printf("Bano ocupado por mujeres\n");
			listo = 1;
		}
			
		else if (estado == MUJERES) {
			printf("Entra una mujer (%d en espera)\n",--espera_mujeres);
			++personas_dentro;
			listo = 1;
		}

		pthread_mutex_unlock(&bano_mutex);

	}
}

void mujer_sale(){

	pthread_mutex_lock(&bano_mutex);
	printf("Sale una mujer\n");
	--personas_dentro;
	if (personas_dentro == 0){
		estado = VACIO;
		pthread_cond_broadcast(&hombre_cond);
		printf("Sanitario vacio\n");
	}
	pthread_mutex_unlock(&bano_mutex);
}

void *hombre(){
	hombre_quiere_entrar();
	sleep(2);
	hombre_sale();
	pthread_exit(NULL);
}

void hombre_quiere_entrar(){

	pthread_mutex_lock(&hombre_mutex);
	printf("Llega un hombre (%d en espera)\n",++espera_hombres);
	pthread_mutex_unlock(&hombre_mutex);

	int listo = 0;

	while (!listo){

	pthread_mutex_lock(&bano_mutex);

		if (estado == MUJERES){
			pthread_cond_wait(&hombre_cond,&bano_mutex);
		}

		if (estado == VACIO){
			estado = HOMBRES;
			printf("Entra un hombre (%d en espera)\n",--espera_hombres);
			++personas_dentro;
			printf("Bano ocupado por hombres\n");
			listo = 1;

		}
		else if (estado == HOMBRES){
			printf("Entra un hombre (%d en espera)\n",--espera_hombres);
			++personas_dentro;
			listo = 1;
		}

		pthread_mutex_unlock(&bano_mutex);
	}
}

void hombre_sale(){
	pthread_mutex_lock(&bano_mutex);
	--personas_dentro;
	printf("Sale un hombre\n");
	if (personas_dentro == 0){
		estado = VACIO;
		pthread_cond_broadcast(&mujer_cond);
		printf("Sanitario vacio\n");
	}
	pthread_mutex_unlock(&bano_mutex);
}

