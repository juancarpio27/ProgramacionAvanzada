#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 10

int main(){

	printf("++++++++linear++++++++\n");
	srand(time(NULL));

	int *array = (int*)malloc(sizeof(int));
	int i;
	for (i=0;i<N;++i){
		*(array+i) = rand() % 50;
	}

	/**Se mueve linearmente por la variable array**/
	#pragma omp simd linear(array)
	for (i=0;i<N;++i){
		printf("%d ",*(array+i));
	}

	free(array);

	return 0;
}
