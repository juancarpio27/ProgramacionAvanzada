#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_ENCUESTAS 2
#define TOTAL_PREGUNTAS 2
#define TOTAL_OPCIONES 6

typedef struct {

	char *Nombre;
	int Edad;

} Persona;

typedef struct {

	char* Encabezado;
	char** Opciones;
	int* Respuestas;

} Pregunta;

typedef struct {
	Pregunta* preguntas;
} Cuestionario;

void cargarPersonas(Persona*);
void cargarPreguntas(Cuestionario*);
void contestarCuestionario(Persona*, Cuestionario*);
void imprimirFrecuencias(Cuestionario*);

int main() {

	Persona *p = (Persona*)malloc(TOTAL_ENCUESTAS*sizeof(Persona));
	Cuestionario *c = (Cuestionario*)malloc(sizeof(Cuestionario));
	c->preguntas = (Pregunta*)malloc(TOTAL_PREGUNTAS*sizeof(Pregunta));
	cargarPersonas(p);
	cargarPreguntas(c);
	contestarCuestionario(p,c);
	imprimirFrecuencias(c);

	return 0;
}



void cargarPersonas(Persona *arreglo){
	Persona *aux;

	for (aux = arreglo; aux < arreglo + TOTAL_ENCUESTAS; aux++){
		Persona pe;
		pe.Nombre = (char*)malloc(20*sizeof(char));
		printf("Nombre: ");
		scanf("%s", pe.Nombre);
		pe.Edad = 0;
		while (pe.Edad < 18 || pe.Edad > 120){
			printf("Edad: ");
			scanf("%d",&pe.Edad);
		}
		*(aux) = pe;
	}

}

void cargarPreguntas(Cuestionario* c){
	Pregunta *aux;
	for (aux = c->preguntas; aux < c->preguntas + TOTAL_PREGUNTAS; ++aux){
		Pregunta p;
		p.Encabezado = (char*)malloc(60*sizeof(char));
		p.Opciones = (char**)malloc(TOTAL_OPCIONES*sizeof(char*));
		p.Respuestas = (int*)malloc((TOTAL_OPCIONES*6)*sizeof(int));
		int *matriz;
		for (matriz = p.Respuestas+1; matriz < p.Respuestas + (TOTAL_OPCIONES*6); matriz++){
			*(matriz) = 0;
		}

		printf("Pregunta: ");
		scanf("%s", p.Encabezado);

		char **preguntas;
		for (preguntas = p.Opciones; preguntas < p.Opciones + TOTAL_OPCIONES; ++preguntas){
			*(preguntas) = (char*)malloc(50*sizeof(char));
			printf("Opcion %ld: ", -(p.Opciones - preguntas)+1);
			scanf("%s", *(preguntas));
		}

		*(aux) = p;
	}
}

void contestarCuestionario(Persona* p, Cuestionario* c){

	srand(time(NULL));
	Persona* auxPersona;
	for (auxPersona = p; auxPersona < p + TOTAL_ENCUESTAS; ++auxPersona){

		int e = auxPersona->Edad;
		Pregunta * auxPreguntas;
		printf("Respuestas de %s\n", auxPersona->Nombre);
		for (auxPreguntas = c->preguntas; auxPreguntas < c->preguntas + TOTAL_PREGUNTAS; auxPreguntas++){
			printf("Pregunta: %s\n",auxPreguntas->Encabezado);
			char **auxOpciones;
			for (auxOpciones = auxPreguntas->Opciones; auxOpciones < auxPreguntas->Opciones + TOTAL_OPCIONES; ++auxOpciones){
				printf("Opcion %ld: %s\n",-(auxPreguntas->Opciones - auxOpciones)+1,*(auxOpciones));
			}

			int respuesta = rand() % 7;
			printf("La respuesta fue %d\n\n",respuesta);
			if (respuesta<6){
				*(auxPreguntas->Respuestas+respuesta) = *(auxPreguntas->Respuestas+respuesta) + 1;
			}

		}

	}

}

void imprimirFrecuencias(Cuestionario *c){

	printf("\n\n------FRECUENCIAS-------\n\n");
	Pregunta *auxPreguntas;
	for (auxPreguntas = c->preguntas; auxPreguntas < c->preguntas + TOTAL_PREGUNTAS; auxPreguntas++){
		printf("%s\n\nNo Respuesta     Frecuencia\n",auxPreguntas->Encabezado);
		printf("1                      %d\n", *(auxPreguntas->Respuestas));
		printf("2                      %d\n", *(auxPreguntas->Respuestas+1));
		printf("3                      %d\n", *(auxPreguntas->Respuestas+2));
		printf("4                      %d\n", *(auxPreguntas->Respuestas+3));
		printf("5                      %d\n", *(auxPreguntas->Respuestas+4));
		printf("6                      %d\n", *(auxPreguntas->Respuestas+5));
	}

}

