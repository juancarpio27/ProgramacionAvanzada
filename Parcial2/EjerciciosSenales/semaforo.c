#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))

int *map; 
int active = 0;
int vecino;
int i;
int en_rojo = 0;
int en_intermitente = 0;
int estabaEncendido = -1;
int ultimaAccion = 2;

void manejador_semaforo(int id){
		active = 0;
		sleep(1); 
		kill(vecino,SIGUSR1);
		kill(getppid(),SIGUSR1);

}

void cambio_semaforo(int id){
		active = 2;
		//*(map+i+4) = 2;
		alarm(5);
}

void consola_mostrar(int id){
		int j = 0;
		while (j<4 && *(map+j+4) != 2)
			j++;
		printf("Voy a actualizar a j = %d\n",j);
		int next = (j+1)%4;
		printf("Tambien next vale = %d\n",next);
		*(map+4+j) = 0;
		*(map+4+next) = 2;
		printf("Semaforo 0:  %d\n",*(map+4));
		printf("Semaforo 1:  %d\n",*(map+5));
		printf("Semaforo 2:  %d\n",*(map+6));
		printf("Semaforo 3:  %d\n",*(map+7));
}

void poner_en_rojo(int id){
	if (!en_rojo){
		int j = 0;
		for (;j<4;++j){
			kill(*(map+j),SIGTSTP);
		}
		j = 0;
		if (estabaEncendido<0){
			while (*(map+j+4) != 2)
				++j;
		}
		estabaEncendido = j;
		for (j=0;j<4;j++){
			*(map+j+4) = 0;
		}
		en_rojo = 1;
		printf("Estado de los semaforos: \n");
		printf("Semaforo 0:  %d\n",*(map+4));
		printf("Semaforo 1:  %d\n",*(map+5));
		printf("Semaforo 2:  %d\n",*(map+6));
		printf("Semaforo 3:  %d\n",*(map+7));
	}else {

		if (ultimaAccion == 1){
			int j = 0;
			for (j=0;j<4;j++){
				*(map+j+4) = 1;
				kill(*(map+j),SIGTSTP);
			}
			ultimaAccion = 0;
		}
		else {
			*(map+4+estabaEncendido) = 2;
			en_rojo = 0;
			int j = 0;
			//for (;j<4;++j){
			//	printf("Mandando mensaje a proceso %d\n",*(map+j));
			//	kill(*(map+j),SIGUSR2);
			//}
			sleep(1);
			kill(*(map+estabaEncendido),SIGALRM);
			estabaEncendido = -1;
		}
	}

}

void poner_en_intermitente(int id){
	if (!en_intermitente){
		int j = 0;
		for (;j<4;++j){
			kill(*(map+j),SIGINT);
		}
		j = 0;
		if (estabaEncendido<0){
		while (*(map+j+4) != 2)
			++j;
		}
		estabaEncendido = j;
		for (j=0;j<4;j++){
			*(map+j+4) = 1;
		}
		en_intermitente = 1;
		printf("Estado de los semaforos: \n");
		printf("Semaforo 0:  %d\n",*(map+4));
		printf("Semaforo 1:  %d\n",*(map+5));
		printf("Semaforo 2:  %d\n",*(map+6));
		printf("Semaforo 3:  %d\n",*(map+7));
	} else {
		if (ultimaAccion == 0){
			int j = 0;
			for (j=0;j<4;j++){
				*(map+j+4) = 1;
				kill(*(map+j),SIGINT);
			}
			ultimaAccion = 1;
		}
		else {
			int j = 0;
			for (;j<4;++j){
				*(map+4+j) = 0;
			}
			*(map+4+estabaEncendido) = 2;
			en_intermitente = 0;
			j = 0;
			for (;j<4;++j){
			
				kill(*(map+j),SIGUSR2);
			}
			sleep(1);
			kill(*(map+estabaEncendido),SIGALRM);
			estabaEncendido = -1;
		}
	}
}

void cambiar_a_intermitente(int id){
	alarm(0);
	active = 1;
}

void cambiar_a_rojo(int id){
	alarm(0);
	//en_rojo = 1;
	active = 0;
}

void quitar_rojo(int id){
	en_rojo = 0;
}

int main() {

	int fd;
    int result;

    //MEMORIA MAPEADA
    fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (fd == -1) {
    	perror("Error opening file for writing");
    	exit(EXIT_FAILURE);
    }
    result = lseek(fd, FILESIZE-1, SEEK_SET);
    if (result == -1) {
    	close(fd);
    	perror("Error calling lseek() to 'stretch' the file");
    	exit(EXIT_FAILURE);
    }
    result = write(fd, "", 1);
    if (result != 1) {
    	close(fd);
    	perror("Error writing last byte of the file");
    	exit(EXIT_FAILURE);
    }
    map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
    	close(fd);
    	perror("Error mmapping the file");
    	exit(EXIT_FAILURE);
    }

	i = 0;

	//CREACION DE LOS SEMAFOROS
	for (;i<4;i++){

		int pid_child;
		pid_child = fork();
		
		//CODIGO DEL HIJO
		if (pid_child == 0){
			
			/***************
			MEMORY MAP 
			****************/
			fd = open(FILEPATH, O_RDONLY);
		    if (fd == -1) {
				perror("Error opening file for reading");
				exit(EXIT_FAILURE);
		    }
		    map = mmap(0, FILESIZE, PROT_READ, MAP_SHARED, fd, 0);
		    if (map == MAP_FAILED) {
				close(fd);
				perror("Error mmapping the file");
				exit(EXIT_FAILURE);
		    }
		    /*********
		    **********/

		    if (signal(SIGALRM,manejador_semaforo) == SIG_ERR){
				printf("No se pudo establecer el manejador de la senal.....FAIL\n");
			}
			if (signal(SIGUSR1,cambio_semaforo) == SIG_ERR){
				printf("No se pudo establecer el manejador de la senal.....FAIL\n");
			}
			if (signal(SIGUSR2,quitar_rojo) == SIG_ERR){
				printf("No se pudo establecer el manejador de la senal.....FAIL\n");
			}
			if (signal(SIGTSTP,cambiar_a_rojo) == SIG_ERR){
				printf("No se pudo establecer el manejador de la senal.....FAIL\n");
			}
			if (signal(SIGINT,cambiar_a_intermitente) == SIG_ERR){
				printf("No se pudo establecer el manejador de la senal.....FAIL\n");
			}

		    int next = (i+1) % 4;
		    vecino = *(map + next);
		    sleep(1);
		    printf("Yo soy el semaforo %d y mi vecino tiene pid %d\n",getpid(),vecino);
		    if (i==0){
		    	active = 2;
		    	alarm(5);
		    }
  			
		 	while(1);

			exit(0);
		}

		//CODIGO DEL PADRE
		else {

			if (signal(SIGUSR1,consola_mostrar) == SIG_ERR){
				printf("No se pudo establecer el manejador de la senal.....FAIL\n");
			}
			if (signal(SIGTSTP,poner_en_rojo) == SIG_ERR){
				printf("No se pudo establecer el manejador de la senal.....FAIL\n");
			}
			if (signal(SIGINT,poner_en_intermitente) == SIG_ERR){
				printf("No se pudo establecer el manejador de la senal.....FAIL\n");
			}

			*(map+i) = pid_child;
			int j = 0;
			for (;j<4;++j)
				*(map+j+4) = 0;
			*(map+4) = 2;

		}

	}

	while (1);

	return 0;
}
