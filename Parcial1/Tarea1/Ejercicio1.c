#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10

typedef struct {

	char* Nombre;
	char* Apellidos;
	int Edad;
	char* Rol;

} Persona;

typedef struct {

	int Codigo;
	char* Nombre;
	float Eslora;
	float Manga;
	int MaximoTripulanes;
	int TripulantesActuales;
	Persona Propietario;
	Persona *Tripulacion;

} Embarcacion;

typedef struct {

	Embarcacion *barcos;
	int tamano;
	int guardados;

} Puerto;

void incoporarEmbarcacion(Puerto*);
void limpiarPuerto(Puerto*);
void imprimirBarco(Embarcacion);
void imprimirPuerto(Puerto*);
void agregarTripulante(Puerto*);
void tripulantes(Puerto*, int);

int main() {

	Puerto *p = (Puerto*) malloc(sizeof(Puerto));
	p->barcos = (Embarcacion*) malloc(N * sizeof(Embarcacion));
	p->tamano = N;
	p->guardados = 0;

	incoporarEmbarcacion(p);
	agregarTripulante(p);
	tripulantes(p,0);

	limpiarPuerto(p);

	return 0;
}

void limpiarPuerto(Puerto *p){

	Embarcacion *aux = p->barcos;
	while (aux < p->barcos + p->guardados){
		Persona *auxT = aux->Tripulacion;
		while (auxT < aux->Tripulacion + aux->TripulantesActuales){
			free(auxT->Nombre);
			free(auxT->Apellidos);
			free(auxT->Rol);
			auxT++;
		}
		free(aux->Propietario.Nombre);
		free(aux->Propietario.Apellidos);
		free(aux->Propietario.Rol);
		free(aux->Nombre);
		free(aux->Tripulacion);
		aux++;
	}
	free(p->barcos);
	free(p);
}

void imprimirBarco(Embarcacion e){
	printf("Barco:\n");
	printf("Nombre: %s\n", e.Nombre);
	printf("Eslora: %f  / Manga: %f\n", e.Eslora, e.Manga);
	printf("Vacantes: %d\n", e.MaximoTripulanes - e.TripulantesActuales);
	printf("Tripulacion: \nNombre: %s / Apellidos: %s / Rol: Propietario\n", e.Propietario.Nombre, e.Propietario.Apellidos);
	Persona *aux;
	for (aux = e.Tripulacion; aux < e.Tripulacion + e.TripulantesActuales; ++aux){
		printf("Nombre: %s / Apellido: %s / Rol: %s\n", aux->Nombre,aux->Apellidos,aux->Rol);
	}
	printf("\n\n");
}

void imprimirPuerto(Puerto *p){

	Embarcacion* aux = p->barcos;
	for (; aux < p->barcos + p->guardados; ++aux){
		imprimirBarco(*aux);
	}

}

void incoporarEmbarcacion(Puerto *p){

	//Lectura de la información del barco
	Embarcacion e;
	e.Codigo = p->guardados;
	e.Nombre = (char*)malloc(20*sizeof(char));
	printf("Informacion del barco:\n");
	printf("Nombre: ");
	scanf("%s", e.Nombre);
	printf("Eslora: ");
	scanf("%f", &e.Eslora);
	printf("Manga: ");
	scanf("%f", &e.Manga);
	printf("Maximo tripulantes: ");
	scanf("%d", &e.MaximoTripulanes);
	e.TripulantesActuales = 0;
	e.Tripulacion = (Persona*)malloc(e.MaximoTripulanes*sizeof(Persona));
	printf("Propietatio: ");
	Persona pe;
	pe.Nombre = (char*)malloc(20*sizeof(char));
	pe.Apellidos = (char*)malloc(20*sizeof(char));
	pe.Rol = (char*)malloc(20*sizeof(char));
	printf("Nombre: ");
	scanf("%s", pe.Nombre);
	printf("Apellidos: ");
	scanf("%s", pe.Apellidos);
	printf("Edad: ");
	scanf("%d", &pe.Edad);
	strcpy(pe.Rol, "Propietario");
	e.Propietario = pe;

	if (p->guardados < p->tamano){
		*(p->barcos + p->guardados) = e;
		p->guardados++;
	}
	else {
		p->barcos = (Embarcacion*)realloc(p->barcos,p->tamano*2*sizeof(Embarcacion));
		p->tamano = p->tamano*2;
		*(p->barcos + p->guardados) = e;
		p->guardados++;
	}

}

void agregarTripulante(Puerto *p){
	Persona pe;
	pe.Nombre = (char*)malloc(20*sizeof(char));
	pe.Apellidos = (char*)malloc(20*sizeof(char));
	pe.Rol = (char*)malloc(20*sizeof(char));
	printf("Agregar tripulante: \nNombre: ");
	scanf("%s", pe.Nombre);
	printf("Apellidos: ");
	scanf("%s", pe.Apellidos);
	printf("Edad: ");
	scanf("%d", &pe.Edad);
	printf("Rol: ");
	scanf("%s", pe.Rol);

	int codigo;
	printf("Embarcacion a la que se desea asignar: ");
	scanf("%d",&codigo);

	Embarcacion *aux = p->barcos;

	while (aux < p->barcos + p->guardados && aux->Codigo != codigo){
		++aux;
	}

	if (aux == p->barcos + p->guardados){
		printf("Error no existe el barco\n");
		free(pe.Nombre);
		free(pe.Apellidos);
		free(pe.Rol);
	}

	else {
		if (aux->TripulantesActuales < aux->MaximoTripulanes){
			*(aux->Tripulacion + aux->TripulantesActuales) = pe;
			aux->TripulantesActuales++;
		}
		else {
			printf("Error el barco esta lleno\n");
			free(pe.Nombre);
			free(pe.Apellidos);
			free(pe.Rol);
		}
	}

}

void tripulantes(Puerto* p , int codigo) {
	Embarcacion *aux = p->barcos;

	while (aux < p->barcos + p->guardados && aux->Codigo != codigo){
		++aux;
	}
	if (aux == p->barcos + p->guardados){
		printf("Error no existe el barco\n");
	}
	else {
		imprimirBarco(*aux);
	}

}