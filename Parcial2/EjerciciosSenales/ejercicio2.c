#include <stdio.h>
#include <signal.h>

void manejador (int id){
	if (id == 2)
		printf("Estoy ignorando al CTRL + C\n");
	else if (id == 18)
		printf("Estoy ignorando al CTRL + Z\n");
}

int main() {

	int child_pid;
	if (signal(SIGINT,manejador) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal.....FAIL\n");
	}
	if (signal(SIGTSTP,manejador) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal....FAIL\n");
	}

	int segundos = 10;
	while (segundos > 0){
		printf("%d\n",segundos);
		segundos--;
		sleep(1);
	}

	return 0;
}