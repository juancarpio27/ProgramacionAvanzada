#include <stdio.h>
#include <signal.h>

void manejador_int (int id){
	printf("....Si apretas Ctrl+C solo vas a matar al padre....\n");
}

void manejador_kill (int id){
	printf("....Nosotros ignoramos al Ctrl+Z....\n");
}

int main() {

	int child_pid;
	if (signal(SIGINT,manejador_int) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal.....FAIL\n");
	}
	if (signal(SIGTSTP,manejador_kill) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal....FAIL\n");
	}

	child_pid = fork();
	if (child_pid < 0){
		printf("Error al crear el proceso\n");
	}
	else if (child_pid == 0){
	
		sleep(2);
		printf("Mi pid es %d y el de mi padre es %d\n",getpid(),getppid());
		int segundos = 10;
		while (segundos > 0){
			printf("%d\n",segundos);
			segundos--;
			sleep(1);
		}
		printf("Mi pid es %d y el de mi padre es %d\n",getpid(),getppid());
	}
	else {
		printf("Soy el padre y mi pid es %d\n",getpid());
		sleep(10);
		wait();

	}

	return 0;
}