#include <omp.h>
#include <stdio.h>
#include <time.h>

#define N 100000

int main(){


	int tid;
	int i;

	float A[N], B[N], S[N], P[N];

	for (i=0;i<N;++i){
		A[i] = i * 0.75;
		B[i] = A[i] * 0.3333;
	}

	double start = omp_get_wtime();

	#pragma omp parallel private(tid,i)
	{
		#pragma omp for
		for (i=0;i<N;++i){
			S[i] = A[i] + B[i];
			P[i] = A[i] * B[i];
		}
	}

	double finish = omp_get_wtime();

	printf("Tiempo total: %f\n",finish-start);

	return 0;
}