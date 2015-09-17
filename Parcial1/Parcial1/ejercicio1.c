#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#define EDIFICIO 0
#define TORRE 1
#define NAVE 2

typedef struct 
{
	int tipo;
	int niveles; //NAVE = 1
	int *modales;
	int periodo;

	//EDIFICIO
	int simetrico;

	//TORRE
	int *diametros; //TAMANO NIVELES*2

	//NAVE
	char *techo;


} Modelo;

typedef struct 
{
	int nomina;
	char *nombre;
	char *apellidos;
	char *fecha;
	char *ingreso;
	int salario;
	char *puesto;

	Modelo *modelos;

	int limite;
	int actuales;

} Trabajador;

typedef struct {
	char *fecha;
	Trabajador *trabajadores;
	int limite;
	int actuales;
} Fecha;

typedef struct {
	Fecha *fecha;
	int limite;
	int actuales;
} Modelos;
 
typedef struct {
	Trabajador *trabajadores;
	int limite;
	int actuales;
} RRHH;

typedef struct {
	Modelos m;
	RRHH r;
} Organizacion;


void agregarTrabajador(Organizacion*);
void imprimirTrabajador(Trabajador);
void imprimirTrabajadores(Organizacion*);

int main(int argc, const char * argv[]){

	Organizacion *o = (Organizacion*)malloc(sizeof(Organizacion));

	o->r.trabajadores = (Trabajador*)malloc(20*sizeof(Trabajador));
	o->r.limite = 20;
	o->r.actuales = 0;

	o->m.fecha = (Fecha*)malloc(20*sizeof(Fecha));
	o->m.limite = 20;
	o->m.actuales = 0;

	agregarTrabajador(o);
	imprimirTrabajadores(o);

	return 0;
}

void agregarTrabajador(Organizacion *o){

	Trabajador t;
	printf("Numero de nomina: ");
	t.nombre = (char*)malloc(50*sizeof(char));
	scanf("%d",&t.nomina);
	printf("Nombre: ");
	scanf("%s",t.nombre);

	t.modelos = (Modelo*)malloc(20*sizeof(Modelo));
	t.limite = 20;
	t.actuales = 0;

	int a = o->r.actuales;

	if (a < o->r.limite){
		*(o->r.trabajadores+a) = t;
		++o->r.actuales;
	}
	else {
		o->r.trabajadores = (Trabajador*)realloc(o->r.trabajadores,2*o->r.actuales);
	}
}

void imprimirTrabajador(Trabajador t){
	printf("Numero de nomina %d / Nombre: %s\n\n",t.nomina,t.nombre);
}

void imprimirTrabajadores(Organizacion *o){
	Trabajador *aux;
	for (aux = o->r.trabajadores; aux < o->r.trabajadores + o->r.actuales; ++aux){
		imprimirTrabajador(*aux);
	}
}