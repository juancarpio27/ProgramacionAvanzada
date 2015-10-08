#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/**Manejar senales
SIG_IGN simplemente ignora el manejador
SIG_DFL pone su funcion POR DEFACTO
if (signal(SIGINT,manejador) == SIG_ERR){
	printf("No se pudo establecer el manejador de la senal.....FAIL\n");
}**/

//SIGINFO una random por ahi
//SIGUSR1 
//SIGUSR2
//SIGALRM
//SIGINT ctrl + C
//SIGTSTP ctrl + Z


int clicks_c;
int child_pid;
int clicks_z;
int x;

void imprimir_clicks(int id){
	printf("Se ha pulsado %d veces CTRL+C y se ha pulsado %d veces CTRL+Z. Acabando…\n",clicks_c,clicks_z);
	if (signal(SIGINT,SIG_DFL) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal.....FAIL\n");
	}
	kill(child_pid,SIGINT);
	kill(getpid(),SIGINT);
}
void sumar_c_z(int id){
	if (id == 2){
		++clicks_c;
		++x;
	}
	else if(id == 18){
		++clicks_z;
		if (x>1){
			--x;
		}
	}
}

void alarma(int id){
	printf("Aparezco cada %d segundos\n",x);
	alarm(x);
}


int main() {


	clicks_c = 0;
	clicks_z = 0;
	x = 3;
	

	child_pid = fork();

	//Codigo de error
	if (child_pid < 0){
		printf("Error\n");
		abort();
	}

	//Codigo del hijo
	if (child_pid == 0){

		if (signal(SIGINT,SIG_IGN) == SIG_ERR){
			printf("No se pudo establecer el manejador de la senal.....FAIL\n");
		}
		if (signal(SIGTSTP,SIG_IGN) == SIG_ERR){
			printf("No se pudo establecer el manejador de la senal.....FAIL\n");
		}
			
		sleep(10);
		kill(getppid(),SIGUSR1);
		exit(0);
	}

	//Codigo del padre
	else {
		if (signal(SIGUSR1,imprimir_clicks) == SIG_ERR){
			printf("No se pudo establecer el manejador de la senal.....FAIL\n");
		}
		if (signal(SIGINT,sumar_c_z) == SIG_ERR){
			printf("No se pudo establecer el manejador de la senal.....FAIL\n");
		}
		if (signal(SIGTSTP,sumar_c_z) == SIG_ERR){
			printf("No se pudo establecer el manejador de la senal.....FAIL\n");
		}
		if (signal(SIGALRM,alarma) == SIG_ERR){
			printf("No se pudo establecer el manejador de la senal.....FAIL\n");
		}
		alarm(3);
		wait(0);			
	}

	return 0;
}

