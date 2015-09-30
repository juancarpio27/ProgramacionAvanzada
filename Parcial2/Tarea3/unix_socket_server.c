#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TCP_PORT 8000

int main(int argc, const char * argv[]) {
    
    struct sockaddr_un direccion;
    char buffer[1000];
    
    int servidor, cliente;
    
    int leidos, escritos;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }

    const char* const socket_name = argv[1];
    
    servidor = socket(PF_LOCAL, SOCK_STREAM, 0);
    
    direccion.sun_family = AF_LOCAL;
    strcpy(direccion.sun_path,socket_name);
    
    bind(servidor,&direccion, SUN_LEN(&direccion));
    
    // Escuchar a los clientes
    listen(servidor, 10);
    
    // Comunicación
    socklen_t tamano = sizeof(direccion);
    
    cliente = accept(servidor, (struct sockaddr *) &direccion, &tamano);
    
    if (cliente >= 0) {
        
        printf("Comunicacion establecida con socket UNIX\n");
        while (leidos = read(cliente, &buffer, sizeof(buffer))) {

            if (!strcmp(buffer,"quit\n"))
                break;
            write(fileno(stdout), &buffer, leidos);
            leidos = read(fileno(stdin), &buffer, sizeof(buffer));
            write(cliente, &buffer, leidos);
            if (!strcmp(buffer,"quit\n"))
                break;
        }     
    }
    
    // Cerrar el socket
    close(servidor);
    close(cliente);
    
    return 0;
}