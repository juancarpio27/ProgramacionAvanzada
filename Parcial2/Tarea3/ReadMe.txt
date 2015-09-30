Instrucciones para usar los programas

Ejercicio 1
daemon.c

Compilar y el ejecutable correr como ./ejecutable -m [minutos]. El demonio se correra cada [m] minutos. Este programa solo se debe correr en linux ya que se apoya de la carpeta /proc

Ejercicio 2

Archivos 

shared_memory_1.c
shared_memory_2.c 

Compilar cada uno, generando exec1 y exec2 
Correr primero exec1 y luego exec2 
En exec1 escribir el numero a calcular el factorial, en la terminal con exec2 se muestra el resultado. Si se manda un -1 ambos programas terminan

Ejercicio 3

Archivos 

semaphore_1.c
semaphore_2.c

Compilar cada uno, generando exec1 y exec2 
Correr primero exec1 y luego exec2 
En exec1 escribir el numero a calcular el factorial, en la terminal con exec2 se muestra el resultado. Si se manda un -1 ambos programas terminan

Ejercicio 4

mapped_memory_1.c
mapped_memory_2.c

Compilar cada uno, generando exec1 y exec2 
Correr primero exec1 y luego exec2 
En exec1 escribir el numero a calcular el factorial, en la terminal con exec2 se muestra el resultado. Si se manda un -1 ambos programas terminan

Ejercicio 5

unix_socket_server.c
unix_socket_client.c

Compilar cada uno, generando exec1 y exec2 
Correr primero exec1 y luego exec2. Ambos deben correr con el mismo archivo /tmp/socket como entrada de la forma, debido a que estos trabajan con archivos al ser en una sola computadora
./exec1 /tmp/socket 
./exec2 /tmp/socket
Empezar a hablar desde el client (exec2) y mandar un mensaje desde cada una intercalado. Si se manda el mensaje quit desde cualquiera de los dos el programa termina





