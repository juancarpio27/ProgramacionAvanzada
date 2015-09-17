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
	Fecha *fechas;
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
void editarTrabajador(Organizacion *o, int nomina);
void agregarEstructura(Organizacion *o, int n);
int existeFecha(Organizacion *o,char * c);
void imprimirOrganizacion(Organizacion *o);
void imprimirFecha(Fecha f);

int main(int argc, const char * argv[]){

	Organizacion *o = (Organizacion*)malloc(sizeof(Organizacion));

	o->r.trabajadores = (Trabajador*)malloc(20*sizeof(Trabajador));
	o->r.limite = 20;
	o->r.actuales = 0;

	o->m.fechas = (Fecha*)malloc(20*sizeof(Fecha));
	o->m.limite = 20;
	o->m.actuales = 0;

	agregarTrabajador(o);
	//editarTrabajador(o,4);
	//imprimirTrabajadores(o);

	agregarEstructura(o,4);
	agregarEstructura(o,4);
	imprimirOrganizacion(o);

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

void editarTrabajador(Organizacion *o, int nomina){
	Trabajador *aux = o->r.trabajadores;
	while (aux < o->r.trabajadores + o->r.actuales && aux->nomina != nomina){
		aux++;
	}
	if (aux == o->r.trabajadores + o->r.actuales){
		printf("Error");
	}
	else {
		printf("Nombre: ");
		scanf("%s",aux->nombre);
	}

}

void imprimirTrabajador(Trabajador t){
	printf("Numero de nomina %d / Nombre: %s\n\n",t.nomina,t.nombre);
}

void imprimirTrabajadores(Organizacion *o){
	Trabajador *aux;
	for (aux = o->r.trabajadores; aux < o->r.trabajadores + o->r.actuales; ++aux){
		imprimirTrabajador(*aux);
		Modelo *aux_modelo = aux->modelos;
		for (aux_modelo = aux->modelos; aux_modelo < aux->modelos + aux->actuales; ++aux_modelo){
			printf("Trabajo de tipo %d\n", aux_modelo->tipo);
		}
	}
}


int existeFecha(Organizacion *o,char * c) {
	Fecha *aux = o->m.fechas;
	while (aux < o->m.fechas + o->m.actuales) {
		if (!strcmp(c,aux->fecha))
			return 1;
	}
	return 0;
}

void agregarEstructura(Organizacion *o, int n){

	char *fecha_obra = (char*)malloc(20*sizeof(char));
	printf("Fecha de la obra: ");
	scanf("%s",fecha_obra);


	//ESTOY AGREGANDO EN UNA FECHA QUE YA EXISTE
	if (existeFecha(o,fecha_obra)){
		Fecha *aux_fecha = o->m.fechas;
		//while (!strcmp(fecha_obra, aux_fecha->fecha))
		//	++aux_fecha;
		printf ("Agregandole un modelo a la fecha %s\n", aux_fecha->fecha);
		Trabajador *aux = o->r.trabajadores;
		while  (aux < o->r.trabajadores + o->r.actuales && aux->nomina != n){
			++aux;
		}
		if (aux == o->r.trabajadores + o->r.actuales)
			printf("Trabajador no encontrado\n");
		else {
			//BUSCO EL TRABAJADOR
			printf("Encontre el trabajador\n");
			*(aux_fecha->trabajadores+aux_fecha->actuales) = *aux;
			aux_fecha->actuales++;

			Trabajador t = *aux;

			//CREO SU MODELO
			Modelo m;
			printf("Tipo de modelo: ");
			scanf("%d",&m.tipo);
			*(t.modelos + t.actuales) = m;
			t.actuales++;

		}
	}

	//SI ESTOY AGREGANDO EN UNA FECHA NUEVA
	else {
		Fecha f;
		f.limite = 20;
		f.actuales = 0;
		f.trabajadores = (Trabajador*)malloc(20*sizeof(Trabajador));
		f.fecha = (char*)malloc(20*sizeof(char));
		strcpy(f.fecha, fecha_obra);

		
		Trabajador *aux = o->r.trabajadores;
		while  (aux < o->r.trabajadores + o->r.actuales && aux->nomina != n){
			++aux;
		}
		if (aux == o->r.trabajadores + o->r.actuales)
			printf("Trabajador no encontrado\n");
		else {
			//BUSCO EL TRABAJADOR
			printf("Encontre el trabajador\n");
			*(f.trabajadores) = *aux;
			f.actuales++;

			Trabajador t = *aux;

			//CREO SU MODELO
			Modelo m;
			printf("Tipo de modelo: ");
			scanf("%d",&m.tipo);
			*(t.modelos + t.actuales) = m;
			aux->actuales++;


			if (o->m.actuales < o->m.limite){
				*(o->m.fechas + o->m.actuales) = f;
				++o->m.actuales;
			}
		}
	}
	free(fecha_obra);

}

void imprimirOrganizacion(Organizacion *o) {
	Fecha *aux = o->m.fechas;
	for (aux = o->m.fechas; aux < o->m.fechas + o->m.actuales; ++aux){
		imprimirFecha(*aux);
	}
}

void imprimirFecha(Fecha f){
	printf("Fecha: %s\n",f.fecha);
	Trabajador *aux = f.trabajadores;
	printf("Tengo %d personas\n",f.actuales);
	while (aux < f.trabajadores + f.actuales){
		imprimirTrabajador(*aux);
		++aux;
	}
}

