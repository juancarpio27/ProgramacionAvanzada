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
	agregarTrabajador(o);
	//editarTrabajador(o,4);
	//imprimirTrabajadores(o);

	agregarEstructura(o,4);
	agregarEstructura(o,3);
	imprimirOrganizacion(o);

	free(o->m.fechas);
	free(o->r.trabajadores);
	free(o);

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
	printf("----TRABAJADOR----\n");
	printf("Numero de nomina %d / Nombre: %s\n",t.nomina,t.nombre);
	printf("Numero de trabajos %d\n", t.actuales);
	Modelo *aux;
	printf("---MODELOS---");
	for (aux = t.modelos; aux<t.modelos+t.actuales; ++aux){
		printf("Trabajo:\nTipo: %d\n", aux->tipo);
	}

}

void imprimirTrabajadores(Organizacion *o){
	Trabajador *aux;
	for (aux = o->r.trabajadores; aux < o->r.trabajadores + o->r.actuales; ++aux){
		imprimirTrabajador(*aux);
		
	}
}


int existeFecha(Organizacion *o,char * c) {
	Fecha *aux = o->m.fechas;
	while (aux < o->m.fechas + o->m.actuales) {
		if (!strcmp(c,aux->fecha))
			return 1;
		++aux;
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
		Trabajador *aux = o->r.trabajadores;
		while  (aux < o->r.trabajadores + o->r.actuales && aux->nomina != n){
			++aux;
		}
		if (aux == o->r.trabajadores + o->r.actuales)
			printf("Trabajador no encontrado\n");
		else {
			//EFECTIVAMENTE EL TRABAJADOR EXISTE EN LA NOMINA AL IGUAL QUE LA FECHA
			//BUSCAR EL TRABAJADOR A VER SI ESTA EN LA FECHA (EL TRABAJADOR ESTA GUARDANDO EN AUX)
			
			while (strcmp(aux_fecha->fecha,fecha_obra))
				++aux_fecha;
			//YA TENGO LA FECHA DONDE VOY A AGREGAR, AHORA TENGO QUE BUSCAR SI EL TRABAJADOR ES NUEVO EN LA FECHA O YA ESTA
			Trabajador *aux_trabajador = aux_fecha->trabajadores;
			while (aux_trabajador < aux_fecha->trabajadores + aux_fecha->limite && aux_trabajador->nomina != n){
				aux_trabajador++;
			}
			if (aux_trabajador == aux_fecha->trabajadores + aux_fecha->limite) {
				printf("Es un nuevo trabajador para la fecha");
				Trabajador t;
				t.nomina = n;
				t.nombre = (char*)malloc(20*sizeof(char));
				t.modelos = (Modelo*)malloc(20*sizeof(Modelo));
				t.nombre = aux->nombre;
				t.actuales = 0;

				//CREO SU MODELO
				Modelo m;
				printf("Tipo de modelo: ");
				scanf("%d",&m.tipo);

				*(t.modelos) = m;
				t.actuales++;

				if (aux_fecha->actuales < aux_fecha->limite){
					*(aux_fecha->trabajadores+aux_fecha->actuales) = t;
					aux_fecha->actuales++;
				}
				else {
					aux_fecha->trabajadores = (Trabajador*)realloc(aux_fecha->trabajadores,aux_fecha->limite*2*sizeof(Trabajador));
					aux_fecha->limite *= 2;
					*(aux_fecha->trabajadores+aux_fecha->actuales) = t;
					aux_fecha->actuales++;
				}
			}
			else {
				//CREO SU MODELO
				Modelo m;
				printf("Tipo de modelo: ");
				scanf("%d",&m.tipo);

				if (aux_trabajador->actuales < aux_trabajador->limite){
					*(aux_trabajador->modelos+aux_trabajador->actuales) = m;
					aux_trabajador->actuales++;
				}
				else {
					aux_trabajador->modelos = (Modelo*)realloc(aux_trabajador->modelos,aux_trabajador->limite*2*sizeof(Modelo));
				}

			}

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
		//BUSCO EL TRABAJADOR
		if (aux == o->r.trabajadores + o->r.actuales)
			printf("Trabajador no encontrado\n");
		//YA QUE EXISTE UN TRABAJADOR PARA ESA FECHA
		else {
			
			printf("Si existe el trabajador, vamos a agregarlo a la fecha\n");
			Trabajador t;
			t.nomina = n;
			t.nombre = (char*)malloc(20*sizeof(char));
			t.modelos = (Modelo*)malloc(20*sizeof(Modelo));
			t.nombre = aux->nombre;
			t.actuales = 0;
			t.limite = 20;

			//CREO SU MODELO
			Modelo m;
			printf("Tipo de modelo: ");
			scanf("%d",&m.tipo);

			*(t.modelos) = m;
			t.actuales++;

			*(f.trabajadores) = t;
			f.actuales++;

			//SI LE PUEDO AGREGAR UNA FECHA A LA ORG
			if (o->m.actuales < o->m.limite){
				*(o->m.fechas + o->m.actuales) = f;
				++o->m.actuales;
			}
			else {
				o->m.fechas = (Fecha*)realloc(o->m.fechas, 2*o->m.limite*sizeof(Fecha));
				o->m.limite = o->m.limite * 2;
				*(o->m.fechas + o->m.actuales) = f;
				++o->m.actuales;
			}
		}
	}
	free(fecha_obra);

}

void imprimirOrganizacion(Organizacion *o) {
	printf("---------ORGANIZACION-------------\n\n\n");
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

