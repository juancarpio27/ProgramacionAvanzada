#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>

#define TCP_PORT 8000

int main(int argc, const char * argv[]) {
    
    struct sockaddr_un direccion;
    char buffer[1000];
    const char* const socket_name = argv[1];
    
    int cliente;
    
    int leidos, escritos;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }
    
    //Crear el socket
    cliente = socket(PF_LOCAL, SOCK_STREAM, 0);
    
    // Establecer conexión
    //direccion.sin_port = htons(TCP_PORT);
    direccion.sun_family = AF_LOCAL;
    strcpy(direccion.sun_path,socket_name);
    //inet_aton(argv[1], &direccion.sin_addr);
    
    int estado = connect(cliente,&direccion, SUN_LEN(&direccion));
    // Comunicación
    
    if (estado == 0) {

        printf("Comunicacion establecida con socket UNIX\n");
        while (leidos = read(fileno(stdin), &buffer, sizeof(buffer))) {

            if (!strcmp(buffer,"quit\n"))
                break;
            write(cliente, &buffer, leidos);
            if (!strcmp(buffer,"quit\n"))
                break;
            leidos = read(cliente, &buffer, sizeof(buffer));
            if (!strcmp(buffer,"quit\n"))
                break;
            write(fileno(stdout), &buffer, leidos);
            if (!strcmp(buffer,"quit\n"))
                break;

        }
    }
    
    // Cerrar el socket
    close(cliente);
    
    return 0;
}

