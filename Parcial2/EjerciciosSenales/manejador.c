#include <stdio.h>
#include <signal.h>

void manejador (int id){
	printf("....Oprimiste Ctrl+C....\n");
}

int main(){

	//SIG_IGN simplemente ignora el manejador
	//SIG_DFL pone su funcion POR DEFACTO
	if (signal(SIGINT,manejador) == SIG_ERR){
		printf("No se pudo establecer el manejador de la senal.....FAIL\n");
	}


	//A partir de aqui control C no termina el programa
	int k = 10;

	while (k-- > 0){
		printf("%d...\n",k);
		sleep(2);
	}

}