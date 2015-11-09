#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 3

int main(){

	int A[N][N];
	int B[N][N];
	int C[N][N];

	srand(time(NULL));
	int i,j,k;

	for (i=0;i<N;++i){
		for (j=0;j<N;++j){
			A[i][j] = rand() % 10;
			B[i][j] = rand() % 10;
		}
	}

	#pragma omp simd collapse(2)
	for (i=0;i<N;++i){
		for (j=0;j<N;++j){
			C[i][j] = 0;
			for (k=0;k<N;k++){
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	printf("El resultado de A*B=\n");
	for (i=0;i<N;++i){
		for (j=0;j<N;++j){
			printf("%d ",C[i][j]);
		}
		printf("\n");
	}

	return 1;
}