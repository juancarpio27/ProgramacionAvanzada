#include <omp.h>
#include <stdio.h>
#include <time.h>

#define N 17
#define M 17

void mover(int x,int y);

int main(){

	int x = 0;
	int y = 0;

	mover(x,y);

	return 0;
}

void mover(int x,int y){

	printf("Roboto en la posicion %d/%d\n",x,y);
	if (x>N && y>M){
		printf("Ya llegue a mi destino");
		return;
	}
	else {
		mover(x+3,y);
		mover(x,y+2);
	}


}



