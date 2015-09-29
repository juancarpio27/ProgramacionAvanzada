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

int factorial(int);

int main(){

	union semun  {
		int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;

	int sem_id;
	int semnum = 0;

	sem_id =  semget(65123,2, IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if (sem_id == -1){
		perror("semget: semget failed"); 
		exit(1);
	}

	while (semctl(sem_id,semnum,GETVAL,arg) != -1){
		if (semctl(sem_id,1,GETVAL,arg) == 1){
			int n;
			n = semctl(sem_id,semnum,GETVAL,arg);
			printf("El factorial de %d es: %d\n",n,factorial(n));
			arg.val = 0;
			semctl(sem_id,1,SETVAL,arg);
		}
	}

	return 0;
}

int factorial(int n){

    int res = 1;
    int i ;
    for (i=1;i<=n;i++){
        res *= i;
    }
    return res;

}