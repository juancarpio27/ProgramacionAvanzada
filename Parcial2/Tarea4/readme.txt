####Ejercicio 1

a) Realice un análisis exhaustivo de los problemas de concurrencia que se pueden dar en un sistema de este tipo, planteando cada uno de ellos y explicándolo.

Los problemas de concurrencia que pueden existir son:
En primer lugar el numero de sillas es menor que el numero de enanitos que hay, por lo que no todos se podran sentar a la vez, deben competir por el recurso de la silla. De igual manera el numero de enanitos que se estan esperando es una variable compartida, entre cada enano y blancanieves, por lo que entre estas dos variables existe el problema de exclusion mutua

Debe existir sincronizacion entre el enanito que se sienta y blancanieves que prepare la comida, y despues de que preparo la comida el enanito debe comer. Por lo que se debe mantener sincronizacion entre estos procesos de forma que se hagan en el orden correcto. Ademas debe haber sincronizacion entre los enanitos y blancanieves de forma que esta no se vaya a dar una vuelta cuando haya un enanito esperando en la mesa

b) Seleccione una técnica de concurrencia para solucionar el problema, explicando los motivos de su selección.

La técnica que escogí fue la utilización de semaforos para proteger las variables compartidad como los enanitos que estan sentados y los enanitos que estan esperando, y de esta forma solucionar el problema de exclusion mutua.

Para solucionar los problemas de sincronizacion utilice variables de condicion. En primer lugar una para que blancanieves se vaya a pasear en caso de que la mesa este vacia, de forma que se pone a esperar una senal de que un enanito se sento. Luego una variable de condicion para indicar que un enano puede sentarse, de forma que si no hay sillas libes se duerma hasta que le llegue una senal de que un enano se paro. Otra variable de condicion para indicarle a blancanieves que ya el enanito se encuentra sentado y ya le puede cocinar, y finalmente una variable de condicion para decirle al enanito que ya puede comer.

c) Programa
Funcion blancanieves y enanito, corren infinitamente, donde el enanito va a trabajar un tiempo aleatorio y luego intenta comer

Ejercicio 2
El main genera varios hombres y varias mujeres, luego de un tiempo genera mas hombres y mujeres de forma aleatoria

Ejercicio 3
El main genera una cantidad de robots con peso aleatorio, y los pisos tambien con peso aleatorio (El peso maximo de un robot es el peso minimo de un piso). Despues se genera otra tanda de robots mas

Ejercici 4
Se pone a correr la consola central y los sensores. Los resultados se escriben en el archivo de texto file.txt
