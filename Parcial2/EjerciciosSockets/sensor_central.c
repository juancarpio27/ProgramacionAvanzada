#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define TCP_PORT 8000

int main(int argc, const char * argv[]) {
    
    struct sockaddr_in direccion;
    char buffer[1000];
    
    int servidor, cliente;
    
    int leidos, escritos;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }
    
    //Crear el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);
    
    // Enlace con el socket
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    inet_aton(argv[1], &direccion.sin_addr);
    
    bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));
    
    // Escuchar a los clientes
    listen(servidor, 10);
    
    // Comunicación
    socklen_t tamano = sizeof(direccion);
    
    while (1){
        cliente = accept(servidor, (struct sockaddr *) &direccion, &tamano);

        int pid_client = fork();

        //CODIGO HIJO
        if (pid_client == 0){
        
            if (cliente >= 0) {
                printf("Aceptando conexiones en %s:%d \n",
                       inet_ntoa(direccion.sin_addr),
                       ntohs(direccion.sin_port));
                // Leer de socket y escribir en pantalla

                while (1) {
                    int *numeros = (int*)malloc(10*sizeof(int));
                    printf("Lei el valor de buffer\n");
                    leidos = read(cliente, &numeros, sizeof(int));
                    
                    //write(fileno(stdout), &buffer, leidos);

                    printf("Imprimendo un numero %d\n",*numeros);
    
                }   
            }

        }

    }
    
    // Cerrar el socket
    
    close(servidor);
    close(cliente);
    
    return 0;
}