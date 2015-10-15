#include <signal.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FICHEROS 5
#define SEGUNDOS 3

/*****COMANDOS
	struct sigaction nombre;
	nombre.sa_handler = ; Aqui va la funcion manejadora, o SIG_IGN o SIG_DFL
	nombre.sa_flags = ; SA_ONESHOT (que se hago una vez), SA_RESTART (Muchas), 
						SA_NOCLDSTOP si la senal es SIGCHLD, no recibe notificaciones de cuando los hijos paren
						SA_NOMASK, ni idea

	sigaction(SENAL,&nombre,0) Pone a la SENAL el comportamiento de &nombre
	sigaction(SENAL,0,&nombre) Pone el comportamiento de SENAL en &nombre
	pause() Se queda esperando una senal 
	kill(pid,SENAL) manda una senal a proceso con pid
	raise(SENAL) manda una senal a si mismo 

	Conjuntos de senales
	sigset_t conjunto;

	sigemptyset(&conjunto); //Conjunto vacio
	sigfillset(&conjunto); //Conjunto con todas las senales
    sigaddset(&conjunto, SIGINT); //Agregar senal a conjunto
    sigdelset(&conjutno, SIGINT); //Borra la senal del conjunto

	sigprocmask(SIG_BLOCK, &conjunto, NULL); //Bloquea conjunto de senales
	sigprocmask(SIG_UNBLOCK, &conjunto, NULL); //Desbloquea conjunto de senales
	sigprocmask(SIG_SETMASK, &conjuto, NULL); //El conjunto de senales bloquedas pasa a conjunto

	sigpending(&pendientes); //Guarda en el conjunto pendientes cuales son las senales que estan pendientes por ejecutar
	sigismember(&pendientes, SIGINT) //Revisa si la senal pertenece al conjunto

******/

int grabar = 1;

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void manejador_alarma(int id){
	grabar = 0;
}

int main(){

	sigset_t todas;
	sigfillset(&todas);
	sigdelset(&todas,SIGALRM);
	sigprocmask(SIG_BLOCK,&todas,NULL);
	struct sigaction nombre;
	nombre.sa_handler = manejador_alarma;
	nombre.sa_flags = SA_RESTART;
	sigaction(SIGALRM,&nombre,0);


	struct dirent *pDirent;
    DIR *pDir;

    pDir = opendir ("datos");
    if (pDir == NULL) {
        system("mkdir datos");
    }
    else {
    	system("rm -rf datos");
    	system("mkdir datos");
    }

    int i = 0;
    for (;i<FICHEROS;++i){
    	grabar = 1;
    	
    	if (i==0){
    		char nombre[20];
    		strcpy(nombre,"/datos/a0");
    		printf("Cree el archivo %s\n",nombre);
    		FILE *f;
    		f = fopen(nombre, "w");
    		if (f == NULL)
    			printf("Error\n");
    		alarm(SEGUNDOS);

    		//while (grabar){
    		//	fprintf(f, "x%s", "");
    		//}
    		sigset_t pendientes;
    		sigpending(&pendientes);
    	}
    	
    }

    printf("Imprimiendo los archivos\n");
    system("ls -l ./datos");


	return 0;
}