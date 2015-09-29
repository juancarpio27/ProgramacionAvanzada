#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

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

	shmctl (segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;
	printf ("segment size: %d\n", segment_size);

	sprintf (shared_memory,"*");

	while (strcmp(shared_memory,"-1")){
		char *s = (char*)malloc(20*sizeof(char));
		printf("Introduzca un valor: ");
		scanf("%s",s);
		sprintf (shared_memory,s);
		free(s);
	}

	shmdt (shared_memory);
	shmctl (segment_id, IPC_RMID, 0);

	

}