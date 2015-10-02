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

int main(int argc, char *argv[]) {
    int i;
    int fd;
    int result;
    int *map;  /* mmapped array of int's */

    fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    if (fd == -1) {
    	perror("Error opening file for writing");
    	exit(EXIT_FAILURE);
    }

    /* Stretch the file size to the size of the (mmapped) array of ints
     */
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
    
    int n = 0;
    int j = 0;
    while (n>=0){
        printf("Numero a calcular: ");
        scanf("%d",&n);
        *(map+j) = n;
        if (n == -1)
            break;
        ++j;
    }

    if (munmap(map, FILESIZE) == -1) {
	perror("Error un-mmapping the file");
    }

    close(fd);
    return 0;
}