#include <math.h>
#include "mpi.h"
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

#define N 8
#define HILOS 4

typedef struct {
	int x;
	int y;
} Move;

typedef struct RobotT {
	int x;
	int y;
	struct RobotT *r;
	int steps;
} Robot;

int myResult = 0;

void printBoard(int *b);
int moveValid(Move m, int *b);
Move *getMoves(Robot *r, int *b);
int isFinal(Move m);
void execute(Robot *r, int *b);
void divideBoard(int *b, int numprocs);
void manejadorSenal(int id);

int main(int argc, char *argv[]){

	int ready = 0;

	char hostname[MPI_MAX_PROCESSOR_NAME];
	int myid, numprocs, nh, tid;
	int  longitud;
	int *x_start = (int*)malloc(sizeof(int));
	int *y_start = (int*)malloc(sizeof(int));

	MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    if (myid == 0){
    	if (signal(SIGUSR1,manejadorSenal) == SIG_ERR){
			printf("No se pudo establecer el manejador de la senal....\n");
		}
    }
    
    MPI_Get_processor_name(hostname, &longitud);

	int *b = (int*)malloc(N*N*sizeof(int));
	int i;
	for (i=0;i<N*N;++i){
		*(b+i) = 0;
	}

	Robot *r = (Robot*)malloc(sizeof(Robot));
	r->x = 0;
	r->y = 0;
	r->steps = 0;
	r->r = NULL;

	if (myid == 0){
		printf("I am the boss, I divide the board\n");
		divideBoard(b,numprocs);
	}
	MPI_Recv(x_start,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	MPI_Recv(y_start,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

	MPI_Bcast(b, N*N, MPI_INT, 0, MPI_COMM_WORLD);
	r->x = *x_start;
	r->y = *y_start;

	printf("I am the process %d and I have my board\n",myid);

	execute(r,b);

	MPI_Reduce(&myResult, &ready, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	free(b);
	free(r);
	free(x_start);
	free(y_start);

	MPI_Finalize();

	return 0;
}

void printBoard(int *b){
	int i,j;
	for (i=0;i<N;++i){
		for (j=0;j<N;++j){
			if (*(b+N*i+j)==0)
				printf("0 ");
			else 
				printf("1 ");
		}
		printf("\n");
	}
}

int moveValid(Move m, int *b){

	return m.x >= 0 && m.x < N && m.y >= 0 && m.y < N && *(b+m.x) == 0 && *(b+m.y) == 0;
}

Move* getMoves(Robot *r, int *b){

	Move *m = (Move*)malloc(4*sizeof(Move));
	int i;
	for (i=0;i<4;++i){
		Move newMove;
		if (i==0){
			newMove.x = r->x + 1;
			newMove.y = r->y;
			*(m+i) = newMove;
		}
		if (i==1){
			newMove.x = r->x;
			newMove.y = r->y + 1;
			*(m+i) = newMove;
		}
		if (i==2){
			newMove.x = r->x - 1;
			newMove.y = r->y;
			*(m+i) = newMove;
		}
		if (i==3){
			newMove.x = r->x;
			newMove.y = r->y - 1;
			*(m+i) = newMove;
		}
	}
	return m;
}

int isFinal(Move m){
	return m.x == N-1 && m.y == N-1;
}

void execute(Robot *r, int *b){

	Move *m = getMoves(r,b);
	int i;
	#pragma omp for private(i)
	for (i=0;i<4;++i){
		Move move = *(m+i);
		if (moveValid(move,b)){
			if (isFinal(move)){
				myResult++;
			}
			else {
				Robot *newRobot = (Robot*)malloc(sizeof(Robot));
				newRobot->x = move.x;
				newRobot->y = move.y;
				newRobot->steps = r->steps+1;
				newRobot->r = r;
				execute(newRobot,b);
				free(newRobot);
			}
		}
	}
	free(m);
}

void divideBoard(int *b, int numprocs){

	int size = sqrt((N*N)/numprocs);
	int blocks = size/2;
	printf ("Size of each board %d and %d blocks per row\n",size,blocks);

	int i;
	int *x = (int*)sizeof(int);
	int *y = (int*)sizeof(int);
	int j = 0;
	*x = 0;

	#pragma omp for private(x,y)
	for (i=0;i<numprocs;++i){
		*y = *x + j*size;
		MPI_Send(x,1,MPI_INT,i,0,MPI_COMM_WORLD);
		MPI_Send(y,1,MPI_INT,i,0,MPI_COMM_WORLD);
		j++;
		if (j == blocks){
			j = 0;
			x = x + size;
		}		
	}

	free(x);
	free(y);
}

void manejadorSenal(int id){
	printf("Imprimo los caminos encontrados hasta el momento\n");
}