#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CRECIMIENTO 5

typedef struct {

	char *nombre;
	char *apellido;
	int edad;
	char *telefono;

} Persona;

typedef struct {

	int ocupada;
	int codigo;
	Persona *paciente;

} Cama;

typedef struct {

	Cama *camas;
	int capacidad;
	int ocupadas;

} Hospital;

Hospital* crearHospital();
void imprimirPaciente(Persona*);
void imprimirHospital(Hospital*);
void ingresarPaciente(Hospital*);
void estadoCama(Hospital*, int);
void darAlta(Hospital*,int);
void disponibilidad(Hospital*);
void liberarMemoria(Hospital*);

int main() {

	Hospital *h = crearHospital();

	ingresarPaciente(h);
	ingresarPaciente(h);
	ingresarPaciente(h);

	imprimirHospital(h);

	darAlta(h,1);

	liberarMemoria(h);

	return 0;
}

Hospital* crearHospital(){

	Hospital *h = (Hospital*)malloc(sizeof(Hospital));
	h->capacidad = CRECIMIENTO;
	h->ocupadas = 0;
	h->camas = (Cama*)malloc(CRECIMIENTO*sizeof(Cama));

	Cama *aux;
	for (aux = h->camas; aux < h->camas + h->capacidad; ++ aux){
		aux->ocupada = 0;
		aux->codigo = aux - h->camas;
	}
	return h;
}

void imprimirPaciente(Persona *p){
	printf("Nombre: %s / Apellido: %s \n\n", p->nombre, p->apellido);
}

void imprimirHospital(Hospital *h){

	printf("Hospital\nCamas libres: %d\nCamas ocupadas: %d\nLista de pacientes\n\n", h->capacidad-h->ocupadas, h->ocupadas);
	Cama *aux;
	for (aux = h->camas; aux < h->camas + h->capacidad; ++ aux){
		printf("Cama no: %d\n", aux->codigo);
		if (aux->ocupada)
			imprimirPaciente(aux->paciente);
		else 
			printf("Vacia\n\n");
	}

}

void ingresarPaciente(Hospital* h){

	printf("Generar ficha paciente: \n");
	Persona *p = (Persona*)malloc(sizeof(Persona));
	printf("Nombre: ");
	p->nombre = (char*)malloc(50*sizeof(char));
	scanf("%s",p->nombre);
	printf("Apellido: ");
	p->apellido = (char*)malloc(50*sizeof(char));
	scanf("%s",p->apellido);
	printf("Edad: ");
	scanf("%d",&p->edad);
	printf("Telefono: ");
	p->telefono = (char*)malloc(50*sizeof(char));
	scanf("%s",p->telefono);

	//Si hay camas en el hospital
	if (h->ocupadas < h->capacidad){
		//Buscar una cama
		Cama *aux = h->camas;
		while (aux < h->camas + h->capacidad && aux->ocupada){
			++aux;
		}
		aux->paciente = p;
		aux->ocupada = 1;
	}
	else {
		h->camas = (Cama*)realloc(h->camas, (h->capacidad+CRECIMIENTO)*sizeof(Cama));
		Cama *aux = h->camas + h->capacidad;
		int i = 0;
		for (; aux < h->camas+h->capacidad+CRECIMIENTO; ++aux){
			aux->ocupada = 0;
			aux->codigo = h->capacidad+i;
			++i;
		}
		aux = h->camas + h->capacidad;
		h->capacidad += CRECIMIENTO;
		aux->paciente = p;
		aux->ocupada = 1;

	}

	h->ocupadas++;

}

void estadoCama(Hospital* h, int n){

	printf("Estado cama no: %d\n",n);
	if (n < h->capacidad){
		Cama *c = h->camas + n;
		printf("Paciente: ");
		if (c->ocupada)
			imprimirPaciente(c->paciente);
		else
			printf("Cama vacia\n");

	}
	else {
		printf("Error cama no existe\n");
	}

}

void darAlta(Hospital* h, int n){
	if (n < h->capacidad){
		Cama *c = h->camas + n;
		if (c->ocupada){
			free(c->paciente->nombre);
			free(c->paciente->apellido);
			free(c->paciente->telefono);
			free(c->paciente);
			c->ocupada = 0;
			h->ocupadas--;
		}
		else {
			printf("La cama ya se encuentra vacia\n");
		}
	}
	else {
		printf("Error cama no existe\n");
	}
}

void disponibilidad(Hospital *h){
	printf("Camas disponibles: %d\n",h->capacidad-h->ocupadas);
	printf("Camas ocupadas: %d\n",h->ocupadas);
}

void liberarMemoria(Hospital *h){
	Cama *c;
	for (c = h->camas; c < h->camas + h->capacidad; ++c){
		if (c->ocupada){
			free(c->paciente->nombre);
			free(c->paciente->apellido);
			free(c->paciente->telefono);
			free(c->paciente);
		}
	}
	free(h->camas);
	free(h);
}

