#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int factorial(int);

int main(){

	int segment_id;
	char *shared_memory;
	struct shmid_ds shmbuffer;
	int segment_size;
	const int shared_segment_size = 0x6400;

	key_t shm_key = 6166529;

	segment_id = shmget(shm_key, shared_segment_size,  IPC_CREAT | S_IRUSR | S_IWUSR);

	shared_memory = (char*) shmat (segment_id, 0, 0);

	printf ("shared memory attached at address %p\n", shared_memory);

	while (1){
		if (strcmp(shared_memory,"*")){
			int n = atoi(shared_memory);
			if (!strcmp(shared_memory,"-1"))
				break;
			printf("El factorial de %d es %d\n",n,factorial(n));
			sprintf (shared_memory,"*");

		}
	}

	shmdt (shared_memory);
	shmctl (segment_id, IPC_RMID, 0);

}

int factorial(int n){

	int res = 1;
	int i ;
	for (i=1;i<=n;i++){
		res *= i;
	}
	return res;

}