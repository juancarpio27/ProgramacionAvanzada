#include <sys/types.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <fcntl.h>
#include <limits.h>


int main (){

	union semun  {
		int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;

	int sem_id;
	int semnum = 0;

	sem_id =  semget(65123,2, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if (sem_id == -1){
		perror("semget: semget failed"); 
		exit(1);
	}

	printf("Semaphore sucesfully created with id: %d\n",sem_id);

	while (1) {

		printf("Numero a calcular el factorial: ");
		scanf("%d",&arg.val);
		semctl(sem_id,0,SETVAL,arg);
		if (arg.val == -1)
			break;
		arg.val = 1;
		semctl(sem_id,1,SETVAL,arg);


	}

	semctl(sem_id, 0, IPC_RMID, arg);

	return 0;
}