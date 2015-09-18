//Examen 1 Juan Andres Carpio A01021224

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
void liberarMemoria(Organizacion*);
void eliminarTrabajador(Organizacion *o, int n);
void imprimirRRHH(Organizacion *o);
void revisarModelosFecha(Organizacion*, int nomina, char *fecha);
void imprimirModelo(Modelo m);
void revisarVariosModelos(Organizacion*, int c);
void reporte(Organizacion*,char*,char*);
int calcularModelos(Fecha f);

int main(int argc, const char * argv[]){

	Organizacion *o = (Organizacion*)malloc(sizeof(Organizacion));

	o->r.trabajadores = (Trabajador*)malloc(20*sizeof(Trabajador));
	o->r.limite = 20;
	o->r.actuales = 0;

	o->m.fechas = (Fecha*)malloc(20*sizeof(Fecha));
	o->m.limite = 20;
	o->m.actuales = 0;

	printf("****Agregar trabajador****\n");
	agregarTrabajador(o);
	printf("****Agregar trabajador****\n");
	agregarTrabajador(o);
	printf("****Agregar trabajador****\n");
	agregarTrabajador(o);

	printf("****Editar trabajador****\n");
	int nomina;
	printf("Nomina: ");
	scanf("%d",&nomina);
	editarTrabajador(o,nomina);

	printf("****Eliminar trabajador****\n");
	printf("Nomina: ");
	scanf("%d",&nomina);
	eliminarTrabajador(o,nomina);

	printf("****Agregar modelo****\n");
	printf("Nomina del trabajador encargado: ");
	scanf("%d",&nomina);
	agregarEstructura(o,nomina);

	printf("****Agregar modelo****\n");
	printf("Nomina del trabajador encargado: ");
	scanf("%d",&nomina);
	agregarEstructura(o,nomina);

	printf("****Agregar modelo****\n");
	printf("Nomina del trabajador encargado: ");
	scanf("%d",&nomina);
	agregarEstructura(o,nomina);

	printf("****Reporte entre los dias 1 y 10****\n");
	reporte(o,"1","10");

	printf("****Imprimir toda la organizacion****\n");
	imprimirOrganizacion(o);

	printf("Revisar modelos para 2 trabajadores simultaneos\n");
	revisarVariosModelos(o,2);
	

	liberarMemoria(o);
	free(o->m.fechas);
	free(o->r.trabajadores);
	free(o);

	return 0;
}

void agregarTrabajador(Organizacion *o){

	Trabajador t;
	printf("Numero de nomina: ");
	scanf("%d",&t.nomina);

	int salario;
	char *puesto;

	t.nombre = (char*)malloc(50*sizeof(char));
	printf("Nombre: ");
	scanf("%s",t.nombre);
	t.apellidos = (char*)malloc(50*sizeof(char));
	printf("Apellido: ");
	scanf("%s",t.apellidos);
	t.fecha = (char*)malloc(50*sizeof(char));
	printf("Fecha de nacimiento: ");
	scanf("%s",t.fecha);
	t.ingreso = (char*)malloc(50*sizeof(char));
	printf("Fecha de ingreso: ");
	scanf("%s",t.ingreso);
	printf("Salario: ");
	scanf("%d",&t.salario);
	t.puesto = (char*)malloc(50*sizeof(char));
	printf("Puesto: ");
	scanf("%s",t.puesto);


	if (o->r.actuales < o->r.limite){
		*(o->r.trabajadores+o->r.actuales) = t;
		++o->r.actuales;
	}
	else {
		o->r.trabajadores = (Trabajador*)realloc(o->r.trabajadores,2*o->r.actuales);
		o->r.limite *= 2;
		*(o->r.trabajadores+o->r.actuales) = t;
		++o->r.actuales;
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
		printf("Apellidos: ");
		scanf("%s",aux->apellidos);
		printf("Fecha de nacimiento: ");
		scanf("%s",aux->fecha);
		printf("Salario: ");
		scanf("%d",&aux->salario);
		printf("Puesto: ");
		scanf("%s",aux->puesto);
	}

}

void imprimirTrabajador(Trabajador t){
	printf("----TRABAJADOR----\n");
	printf("Numero de nomina %d / Nombre y apellido: %s %s\n",t.nomina,t.nombre,t.apellidos);
	printf("Fecha de nacimiento: %s  /  Fecha de ingreso %s\n",t.fecha,t.ingreso);
	printf("Puesto: %s\n",t.puesto);
	printf("Numero de trabajos %d\n\n", t.actuales);
	Modelo *aux;
	printf("---MODELOS---\n");
	for (aux = t.modelos; aux<t.modelos+t.actuales; ++aux){
		imprimirModelo(*aux);
	}
	printf("\n");

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
				t.apellidos = (char*)malloc(20*sizeof(char));
				t.fecha  = (char*)malloc(20*sizeof(char));
				t.ingreso = (char*)malloc(20*sizeof(char));
				t.puesto = (char*)malloc(20*sizeof(char));
				t.modelos = (Modelo*)malloc(20*sizeof(Modelo));
				strcpy(t.nombre,aux->nombre);
				strcpy(t.apellidos,aux->apellidos);
				strcpy(t.fecha,aux->fecha);
				strcpy(t.ingreso,aux->ingreso);
				strcpy(t.puesto,aux->puesto);

				t.actuales = 0;
				t.limite = 20;


				//CREO SU MODELO
				Modelo m;
				printf("Tipo de modelo (0.Edifico 1.Torre 2. Nave): ");
				scanf("%d",&m.tipo);

				printf("Periodo: ");
				scanf("%d",&m.periodo);

				//SE CREO UN 
				if (m.tipo == EDIFICIO){
					printf("Niveles: ");
					scanf("%d", &m.niveles);
					m.modales = (int*)malloc(m.niveles*sizeof(int));
					printf("Simetrico: 1.Si 0.No");
					scanf("%d",&m.simetrico);
				}
				if (m.tipo == TORRE){
					printf("Niveles: ");
					scanf("%d", &m.niveles);
					m.modales = (int*)malloc(m.niveles*sizeof(int));
					m.diametros = (int*)malloc(m.niveles*2*sizeof(int));
				}
				if (m.tipo == NAVE){
					m.niveles = 1;
					m.modales = (int*)malloc(m.niveles*sizeof(int));
					m.techo = (char*)malloc(20*sizeof(char));
					printf("Tipo de techo: ");
					scanf("%s",m.techo);
				}

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
				printf("Tipo de modelo (0.Edifico 1.Torre 2. Nave): ");
				scanf("%d",&m.tipo);

				printf("Periodo: ");
				scanf("%d",&m.periodo);

				//SE CREO UN 
				if (m.tipo == EDIFICIO){
					printf("Niveles: ");
					scanf("%d", &m.niveles);
					m.modales = (int*)malloc(m.niveles*sizeof(int));
					printf("Simetrico: 1.Si 0.No");
					scanf("%d",&m.simetrico);
				}
				if (m.tipo == TORRE){
					printf("Niveles: ");
					scanf("%d", &m.niveles);
					m.modales = (int*)malloc(m.niveles*sizeof(int));
					m.diametros = (int*)malloc(m.niveles*2*sizeof(int));
				}
				if (m.tipo == NAVE){
					m.niveles = 1;
					m.modales = (int*)malloc(m.niveles*sizeof(int));
					m.techo = (char*)malloc(20*sizeof(char));
					printf("Tipo de techo: ");
					scanf("%s",m.techo);
				}

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
		if (aux == o->r.trabajadores + o->r.actuales){
			printf("Trabajador no encontrado\n");
			free(f.fecha);
			free(f.trabajadores);
		}
		//YA QUE EXISTE UN TRABAJADOR PARA ESA FECHA
		else {
			
			printf("Si existe el trabajador, vamos a agregarlo a la fecha\n");
			Trabajador t;
			t.nomina = n;
			t.nombre = (char*)malloc(20*sizeof(char));
			t.apellidos = (char*)malloc(20*sizeof(char));
			t.fecha  = (char*)malloc(20*sizeof(char));
			t.ingreso = (char*)malloc(20*sizeof(char));
			t.puesto = (char*)malloc(20*sizeof(char));
			t.modelos = (Modelo*)malloc(20*sizeof(Modelo));
			strcpy(t.nombre,aux->nombre);
			strcpy(t.apellidos,aux->apellidos);
			strcpy(t.fecha,aux->fecha);
			strcpy(t.ingreso,aux->ingreso);
			strcpy(t.puesto,aux->puesto);

			t.actuales = 0;
			t.limite = 20;

			//CREO SU MODELO
			Modelo m;
			printf("Tipo de modelo (0.Edifico 1.Torre 2. Nave): ");
			scanf("%d",&m.tipo);
			printf("Periodo: ");
			scanf("%d",&m.periodo);

			//SE CREO UN 
			if (m.tipo == EDIFICIO){
				printf("Niveles: ");
				scanf("%d", &m.niveles);
				m.modales = (int*)malloc(m.niveles*sizeof(int));
				printf("Simetrico: 1.Si 0.No");
				scanf("%d",&m.simetrico);
			}
			if (m.tipo == TORRE){
				printf("Niveles: ");
				scanf("%d", &m.niveles);
				m.modales = (int*)malloc(m.niveles*sizeof(int));
				m.diametros = (int*)malloc(m.niveles*2*sizeof(int));
			}
			if (m.tipo == NAVE){
				m.niveles = 1;
				m.modales = (int*)malloc(m.niveles*sizeof(int));
				m.techo = (char*)malloc(20*sizeof(char));
				printf("Tipo de techo: ");
				scanf("%s",m.techo);
			}

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
	printf("------FECHA-------");
	printf("Fecha: %s\n",f.fecha);
	Trabajador *aux = f.trabajadores;
	printf("Tengo %d personas\n",f.actuales);
	while (aux < f.trabajadores + f.actuales){
		imprimirTrabajador(*aux);
		++aux;
	}
	printf("\n");
}

void liberarMemoria(Organizacion* o) {

	Trabajador *aux_trabajador;
	for (aux_trabajador = o->r.trabajadores; aux_trabajador < o->r.trabajadores + o->r.actuales; ++aux_trabajador){
		free(aux_trabajador->nombre);
		free(aux_trabajador->apellidos);
		free(aux_trabajador->fecha);
		free(aux_trabajador->ingreso);
		free(aux_trabajador->puesto);
	}
	Fecha *aux_fecha;
	for (aux_fecha = o->m.fechas; aux_fecha < o->m.fechas + o->m.actuales; ++aux_fecha){
		free(aux_fecha->fecha);
		for (aux_trabajador = aux_fecha->trabajadores; aux_trabajador < aux_fecha->trabajadores + aux_fecha->actuales; ++aux_trabajador){
			//Al ser copias se borraron al darle free arriba
			free(aux_trabajador->nombre);
			free(aux_trabajador->apellidos);
			free(aux_trabajador->fecha);
			free(aux_trabajador->ingreso);
			free(aux_trabajador->puesto);

			Modelo *aux_modelo;
			for (aux_modelo = aux_trabajador->modelos; aux_modelo < aux_trabajador->modelos + aux_trabajador->actuales; ++ aux_modelo){
				free(aux_modelo->modales);
				if (aux_modelo->tipo == TORRE){
					free(aux_modelo->diametros);
				}
				if (aux_modelo->tipo == NAVE){
					free(aux_modelo->techo);
				}
			}

			free(aux_trabajador->modelos);
		}
		free(aux_fecha->trabajadores);
	}


}

void eliminarTrabajador(Organizacion *o, int n){

	Trabajador *aux = o->r.trabajadores;
	while (aux < o->r.trabajadores + o->r.actuales && aux->nomina != n){
		aux++;
	}
	if (aux == o->r.trabajadores + o->r.actuales){
		printf("Error");
	}
	else {
		Trabajador t = *(o->r.trabajadores + o->r.actuales - 1);
		free(aux->nombre);
		free(aux->apellidos);
		free(aux->ingreso);
		free(aux->puesto);
		free(aux->fecha);


		*aux = t;
		o->r.actuales--;

	}

}

void imprimirRRHH(Organizacion *o){

	printf("-----RECURSOS HUMANOS-----");
	Trabajador *aux;
	for (aux=o->r.trabajadores; aux< o->r.trabajadores + o->r.actuales; ++aux){
		printf("Trabajador %s %s",aux->nombre, aux->apellidos);
	}

}

void revisarModelosFecha(Organizacion* o, int nomina, char *fecha){
	//PRIMERO DEBO BUSCAR LA FECHA

	Fecha *aux_fecha = o->m.fechas;
	while (aux_fecha < o->m.fechas + o->m.actuales && strcmp(fecha,aux_fecha->fecha)){
		++aux_fecha;
	}
	if (aux_fecha == o->m.fechas + o->m.actuales)
		printf("Error: no hay trabajos para esta fecha");

	//UNA VEZ QUE CONSEGUI LA FECHA
	else {
		Trabajador *aux_trabajador = aux_fecha->trabajadores;
		while (aux_trabajador < aux_fecha->trabajadores + aux_fecha->actuales && aux_trabajador->nomina != nomina){
			++aux_trabajador;
		}
		if (aux_trabajador == aux_fecha->trabajadores + aux_fecha->actuales)
			printf("Error: no hay trabajador en esta fecha");
		else {
			//Si lo consegui
			Modelo *aux_modelo;
			for (aux_modelo = aux_trabajador->modelos; aux_modelo < aux_trabajador->modelos + aux_trabajador->actuales; ++aux_modelo){
				imprimirModelo(*aux_modelo);
			}
		}
	}
}

void imprimirModelo(Modelo m){

	printf("-----MODELO------\n");
	if (m.tipo == 0){
		printf("Edificio\n");
		printf("Niveles: %d\n",m.niveles);
		printf("periodo: %d\n",m.periodo);
		if (m.simetrico){
			printf("Simetrico\n");
		}
		else {
			printf("No simetrico\n");
		}
	}
	else if (m.tipo == 1){
		printf("Torre\n");
		printf("Niveles %d\n",m.niveles);
		printf("periodo: %d\n",m.periodo);
	}
	else {
		printf("Nave\n");
		printf("Niveles %d\n",1);
		printf("periodo: %d\n",m.periodo);
		printf("Techo: %s\n", m.techo);
	}

}

void revisarVariosModelos(Organizacion* o, int c){

	int i;
	for (i = 0;i<c;i++){
		int nomina;
		printf("Nomina a revisar: ");
		scanf("%d",&nomina);
		printf("fecha a revisar: ");
		char *fecha = (char*)malloc(20*sizeof(char));
		scanf("%s",fecha);
		pid_t childpid;
		childpid = fork();

		if (childpid < 0)
			printf("Error en la creacion del proceso\n");
		else if (childpid == 0){
			revisarModelosFecha(o,nomina,fecha);
			liberarMemoria(o);
			free(o->m.fechas);
			free(o->r.trabajadores);
			free(o);
			free(fecha);
			exit(1);
		}

		free(fecha);
	}

	printf("Lista la revision\n");

}

void reporte(Organizacion* o,char* inicio,char* fin) {

	int fecha_inicio = atoi(inicio);
	int fecha_fin = atoi(fin);

	printf("*******REPORTES CON RANGO %s - %s ********",inicio,fin);

	Fecha *aux_fecha;
	for (aux_fecha = o->m.fechas; aux_fecha < o->m.fechas + o->m.actuales; ++aux_fecha){
		printf("----REPORTE-----\n\n");
		printf("Fecha %s\n\n",aux_fecha->fecha);
		printf("Reportes para este dia: %d\n\n", calcularModelos(*aux_fecha));
		int comp = atoi(aux_fecha->fecha);
		if (fecha_inicio <= comp && comp <= fecha_fin){
			Trabajador *aux_trabajador;
			for (aux_trabajador = aux_fecha->trabajadores; aux_trabajador < aux_fecha->trabajadores + aux_fecha->actuales; ++aux_trabajador){
				imprimirTrabajador(*aux_trabajador);
			}
		}
	}
}

int calcularModelos(Fecha f){
	Trabajador *aux_trabajador;
	int res = 0;
	for (aux_trabajador = f.trabajadores; aux_trabajador < f.trabajadores + f.actuales; aux_trabajador++){
		res += aux_trabajador->actuales;
	}
	return res;
}

