#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))

int factorial(int);

int main(int argc, char *argv[])
{
    int i;
    int fd;
    int *map;  /* mmapped array of int's */

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
    
    int j = 0;
    printf("AQUI ESTOY\n\n");
    while (1){

        if (*(map+j) != 0){
            if (*(map+j) == -1)
                break;
            printf("El factorial de %d es = %d\n",*(map+j),factorial(*(map+j)));
            ++j;
        }
    }

    if (munmap(map, FILESIZE) == -1) {
	perror("Error un-mmapping the file");
    }
    close(fd);
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