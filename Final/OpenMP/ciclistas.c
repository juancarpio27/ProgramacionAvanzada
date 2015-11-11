#include <omp.h>
#include <stdio.h>
#include <time.h>

#define ETAPAS 5
#define LUGARES 2
#define CICLISTAS 6

int main(){


	int puntos[CICLISTAS];
	int i,tid,tiempo;
	srand(time(NULL));
	int llego = 0;

	#pragma omp parallel num_threads(CICLISTAS) private(tid,tiempo) shared(llego)
	{
		for (i=0;i<ETAPAS;++i){
			tid = omp_get_thread_num();
			printf("EMPEZO: etapa %d ciclista %d\n",i,tid);
			//OPERACION
			printf("TERMINO: etapa %d cilcista %d\n",i,tid);
			#pragma omp critical
			{
				if (!llego){
					llego = 1;
					puntos[tid]++;
				}
			}
			#pragma omp barrier 
		}

	}


	return 0;
}
