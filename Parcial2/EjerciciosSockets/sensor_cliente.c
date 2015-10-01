#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define TCP_PORT 8000

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;
    
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[])
{
    int i, sign;
    
    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int main(int argc, const char * argv[]) {
    
    struct sockaddr_in direccion;
    char buffer[1000];
    
    int cliente;
    
    int leidos, escritos;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }
    
    //Crear el socket
    cliente = socket(PF_INET, SOCK_STREAM, 0);
    
    // Establecer conexión
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    inet_aton(argv[1], &direccion.sin_addr);
    
    int estado = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));
    
    // Comunicación
    srand(time(NULL));
    if (estado == 0) {
        printf("Conectado a %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));
        
        // Leer de teclado y escribir en socket
        while (1) {

            int segundos = rand() % 10;
            printf("ME toca mandar mensaje al central en %d segundos",segundos);

            //strcpy(buffer,"");
            char actual[1000];
            strcpy(actual,"");
            int *numeros = (int*)malloc(10*sizeof(int));

            while (segundos >=0) {

                //ME DUERMO UN SEGUNDO
                sleep(1);

                
                int actuales = 0;
                int limite = 10;
                int medida = rand() % 100;
                if (actuales < limite){
                    *(numeros+actuales) = medida;
                    actuales++;
                }
                else {
                    numeros = (int*)realloc(numeros,2*limite*sizeof(int));
                    limite *= 2;
                    *(numeros+actuales) = medida;
                    actuales++;
                }
                itoa(medida,buffer);
                printf("Medi el numero %s\n",buffer);
                //strcat(actual,medida);
                strcat(actual," ");
                segundos--;

            }
            printf("Voy a mandar %s",buffer);
            write(cliente, &numeros, sizeof(numeros));
            free(numeros);

        }
    }
    
    // Cerrar el socket
    close(cliente);
    
    return 0;
}