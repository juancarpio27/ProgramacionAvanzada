#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


void ps();
char* readLine(char* file);
char getStatus(char *line);
//char* getppid(char *line);

typedef struct {
	int pid;
	char state;
	char *ppid;
	char *name;
} Process;

int main(int argc, char **argv) {

	char *m_value = NULL;
    int c = 0;

    while ((c = getopt(argc, argv, "m:")) != -1) {
        switch (c) {
        	case 'm':
                m_value = optarg;
                break;
            case '?':
                return 1;
            default:
                abort();
        }
    }

    int minutes = atoi(m_value);

	pid_t pid, sid;     
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0);

    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    if ((chdir("/")) < 0) {
       	exit(EXIT_FAILURE);
    }

    while (1){
    	sleep(60*minutes);
    	ps();
    }

    return 0;
}

void ps(){
	int len;
    struct dirent *pDirent;
    DIR *pDir;

    pDir = opendir ("/proc");
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n","/proc");
        return;
    }

    printf ("PID\n");
    while ((pDirent = readdir(pDir)) != NULL) {
       	Process p;
        p.pid = atoi(pDirent->d_name);
        if (p.pid != 0) {
  			printf("%d  ",p.pid);
  			char *status = (char*)malloc(50*sizeof(char));
  			strcpy(status,"/proc/");
  			strcat(status,pDirent->d_name);
  			strcat(status,"/status");;
  			FILE *fr;
  			long elapsed_seconds;
  			char line[80];
  			fr = fopen (status,"r"); 
  			//AVANZAR HASTA LA SEGUNDA LINEA DONDE ESTA EL STATUS
  			fgets(line,1000,fr);
  	
  			char* aux_linespace = line;
			while (*aux_linespace != '\n')
				++aux_linespace;
			*aux_linespace='\0';
  			printf("%s ",line+5);

  			fgets(line,1000,fr);
  			//sscanf (line, "%ld", &elapsed_seconds); 
			p.state = getStatus(line);

			fgets(line,1000,fr);
			fgets(line,1000,fr);
			fgets(line,1000,fr);
			fgets(line,1000,fr);

			//char *ppid = getppid(line);

			//printf("%s ",ppid);
			printf("%c ",p.state);
			free(status);
			
			fclose(fr);
		   }
  		printf("\n");
    }
    
    closedir (pDir);
}

char getStatus(char *line){
	char* aux = line;
	return *(aux+7);
}

/*char* getppid(char *line){
	
	char *aux = (char*)malloc(6*sizeof(char));
	aux = line+6;
	char* aux_linespace = aux;
	while (*aux_linespace != '\n')
		++aux_linespace;
	*aux_linespace='\0';
	return aux;
}*/