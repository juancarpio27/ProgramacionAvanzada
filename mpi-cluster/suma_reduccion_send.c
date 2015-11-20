//
//  main.c
//  mpi_suma_reduccion
//
//  Created by Vicente Cubells Nonell on 12/11/15.
//  Copyright © 2015 Vicente Cubells Nonell. All rights reserved.
//

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 1000

int main(int argc, char *argv[])
{
    int myid, numprocs;
    int data[MAXSIZE];
    int i, n, x, low, high, myresult = 0, result = 0;
    double start, stop;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int  longitud;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    
    MPI_Get_processor_name(hostname, &longitud);
    
    n = MAXSIZE;
    /* Inicializar datos */
    if (myid == 0) {
        for(i = 0; i < n; i++) {
            data[i] = i;
        }
    }
    
    if (myid == 0 ) start = MPI_Wtime();
    
    /* Broadcast a todos los procesos */
    //MPI_Bcast(data, n, MPI_INT, 0, MPI_COMM_WORLD);
    
    /* Porción de data  que va a procesar cada procesador*/
    x = n/numprocs;
    low = myid * x;
    high = low + x;

    int *my_array = (int*)malloc(x*sizeof(int));

    if (myid == 0){
        for (i=1;i<numprocs;++i){
            MPI_Send((data+i*x),x,MPI_INT,i,0,MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(my_array,x,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }

    if (myid == numprocs - 1) { high = n; }
    for(i = 0; i < x; i++) {
        // Poner un sleep()
        myresult += my_array[i];
    }
    printf("Yo soy el procesador %d (%s)  y mi suma = %d\n", myid , hostname, myresult);
    
    /* Suma global mediante reducción */
    MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (myid == 0) printf("La suma total del vector es = %d.\n", result);
    
    if (myid == 0) {
        stop = MPI_Wtime();
        printf("Tiempo de ejecución de la sección paralela = %f \n", stop-start);
    }
    
    MPI_Finalize();
    
    return 0;
}