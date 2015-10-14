#include <signal.h>
#include <stdio.h>

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

int main(){

	return 0;
}