#include <stdio.h>
#include <signal.h>

void manejador_uno (int id){
	printf("Estoy aprendiendo a manejar señales...\n");
}

void manejador_dos (int id){
	printf("Ahora cambié de manejador...\n");
}

int main(){

	int child_pid;
	if (signal(SIGINT,manejador_uno) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal.....FAIL\n");
	}

	int segundos = 1;
	while (segundos <= 10){
		printf("Contando la %d ovejita\n",segundos);
		segundos++;
		sleep(1);
	}

	if (signal(SIGINT,manejador_dos) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal.....FAIL\n");
	}

	segundos = 1;
	while (segundos <= 10){
		printf("Contando la %d trineo\n",segundos);
		segundos++;
		sleep(1);
	}


	if (signal(SIGINT,manejador_uno) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal.....FAIL\n");
	}

	segundos = 1;
	while (segundos <= 10){
		printf("Contando la %d cabrita\n",segundos);
		segundos++;
		sleep(1);
	}


	return 0;
}