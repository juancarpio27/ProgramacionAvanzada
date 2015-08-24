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
void imprimirNumero(int);
void imprimirEdades(Cuestionario*);
void liberarMemoria(Persona*,Cuestionario*);

int main() {

	Persona *p = (Persona*)malloc(TOTAL_ENCUESTAS*sizeof(Persona));
	Cuestionario *c = (Cuestionario*)malloc(sizeof(Cuestionario));
	c->preguntas = (Pregunta*)malloc(TOTAL_PREGUNTAS*sizeof(Pregunta));
	cargarPersonas(p);
	cargarPreguntas(c);
	contestarCuestionario(p,c);
	imprimirFrecuencias(c);
	imprimirEdades(c);
	liberarMemoria(p,c);

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
			printf("La respuesta fue %d\n\n",respuesta+1);
			if (respuesta<6){
				*(auxPreguntas->Respuestas+respuesta) = *(auxPreguntas->Respuestas+respuesta) + 1;
				if ( 18 <= auxPersona->Edad && auxPersona->Edad <= 25 ){
					*(auxPreguntas->Respuestas+6+respuesta) = *(auxPreguntas->Respuestas+6+respuesta) + 1;
				}
				else if ( 26 <= auxPersona->Edad && auxPersona->Edad <= 35){
					*(auxPreguntas->Respuestas+6*2+respuesta) = *(auxPreguntas->Respuestas+6*2+respuesta) + 1;
				}
				else if ( 36 <= auxPersona->Edad && auxPersona->Edad <= 45){
					*(auxPreguntas->Respuestas+6*3+respuesta) = *(auxPreguntas->Respuestas+6*3+respuesta) + 1;
				}
				else if ( 46 <= auxPersona->Edad && auxPersona->Edad <= 65){
					*(auxPreguntas->Respuestas+6*4+respuesta) = *(auxPreguntas->Respuestas+6*4+respuesta) + 1;
				}
				else if ( 66 <= auxPersona->Edad && auxPersona->Edad <= 120){
					*(auxPreguntas->Respuestas+6*5+respuesta) = *(auxPreguntas->Respuestas+6*5+respuesta) + 1;
				}
			}

		}

	}

}

void imprimirFrecuencias(Cuestionario *c){

	printf("\n\n------FRECUENCIAS-------\n\n");
	Pregunta *auxPreguntas;
	for (auxPreguntas = c->preguntas; auxPreguntas < c->preguntas + TOTAL_PREGUNTAS; auxPreguntas++){
		printf("%s\n\nNo Respuesta     Frecuencia\n",auxPreguntas->Encabezado);
		printf("1                      %d    ", *(auxPreguntas->Respuestas)); imprimirNumero(*(auxPreguntas->Respuestas));
		printf("2                      %d    ", *(auxPreguntas->Respuestas+1)); imprimirNumero(*(auxPreguntas->Respuestas+1));
		printf("3                      %d    ", *(auxPreguntas->Respuestas+2)); imprimirNumero(*(auxPreguntas->Respuestas+2));
		printf("4                      %d    ", *(auxPreguntas->Respuestas+3)); imprimirNumero(*(auxPreguntas->Respuestas+3));
		printf("5                      %d    ", *(auxPreguntas->Respuestas+4)); imprimirNumero(*(auxPreguntas->Respuestas+4));
		printf("6                      %d    ", *(auxPreguntas->Respuestas+5)); imprimirNumero(*(auxPreguntas->Respuestas+5));
	}
	printf("\n\n");

}

void imprimirEdades(Cuestionario *c){
	printf("\n\n------EDADES-------\n\n");
	Pregunta *auxPreguntas;
	for (auxPreguntas = c->preguntas; auxPreguntas < c->preguntas + TOTAL_PREGUNTAS; auxPreguntas++){
		printf("%s\n",auxPreguntas->Encabezado);
		printf("\n1\n(18-25)        %d     ", *(auxPreguntas->Respuestas+6)); imprimirNumero(*(auxPreguntas->Respuestas+6));
		printf("(26-35)        %d     ", *(auxPreguntas->Respuestas+6*2)); imprimirNumero(*(auxPreguntas->Respuestas+6*2));
		printf("(36-45)        %d     ", *(auxPreguntas->Respuestas+6*3)); imprimirNumero(*(auxPreguntas->Respuestas+6*3));
		printf("(46-65)        %d     ", *(auxPreguntas->Respuestas+6*4)); imprimirNumero(*(auxPreguntas->Respuestas+6*4));
		printf("(66-120)       %d     ", *(auxPreguntas->Respuestas+6*5)); imprimirNumero(*(auxPreguntas->Respuestas+6*5));

		printf("%s\n",auxPreguntas->Encabezado);
		printf("\n2\n(18-25)        %d     ", *(auxPreguntas->Respuestas+6+1)); imprimirNumero(*(auxPreguntas->Respuestas+6+1));
		printf("(26-35)        %d     ", *(auxPreguntas->Respuestas+6*2+1)); imprimirNumero(*(auxPreguntas->Respuestas+6*2+1));
		printf("(36-45)        %d     ", *(auxPreguntas->Respuestas+6*3+1)); imprimirNumero(*(auxPreguntas->Respuestas+6*3+1));
		printf("(46-65)        %d     ", *(auxPreguntas->Respuestas+6*4+1)); imprimirNumero(*(auxPreguntas->Respuestas+6*4+1));
		printf("(66-120)       %d     ", *(auxPreguntas->Respuestas+6*5+1)); imprimirNumero(*(auxPreguntas->Respuestas+6*5+1));

		printf("%s\n",auxPreguntas->Encabezado);
		printf("\n3\n(18-25)        %d     ", *(auxPreguntas->Respuestas+6+2)); imprimirNumero(*(auxPreguntas->Respuestas+6+2));
		printf("(26-35)        %d     ", *(auxPreguntas->Respuestas+6*2+2)); imprimirNumero(*(auxPreguntas->Respuestas+6*2+2));
		printf("(36-45)        %d     ", *(auxPreguntas->Respuestas+6*3+2)); imprimirNumero(*(auxPreguntas->Respuestas+6*3+2));
		printf("(46-65)        %d     ", *(auxPreguntas->Respuestas+6*4+2)); imprimirNumero(*(auxPreguntas->Respuestas+6*4+2));
		printf("(66-120)       %d     ", *(auxPreguntas->Respuestas+6*5+2)); imprimirNumero(*(auxPreguntas->Respuestas+6*5+2));

		printf("%s\n",auxPreguntas->Encabezado);
		printf("\n4\n(18-25)        %d     ", *(auxPreguntas->Respuestas+6+3)); imprimirNumero(*(auxPreguntas->Respuestas+6+3));
		printf("(26-35)        %d     ", *(auxPreguntas->Respuestas+6*2+3)); imprimirNumero(*(auxPreguntas->Respuestas+6*2+3));
		printf("(36-45)        %d     ", *(auxPreguntas->Respuestas+6*3+3)); imprimirNumero(*(auxPreguntas->Respuestas+6*3+3));
		printf("(46-65)        %d     ", *(auxPreguntas->Respuestas+6*4+3)); imprimirNumero(*(auxPreguntas->Respuestas+6*4+3));
		printf("(66-120)       %d     ", *(auxPreguntas->Respuestas+6*5+3)); imprimirNumero(*(auxPreguntas->Respuestas+6*5+3));

		printf("%s\n",auxPreguntas->Encabezado);
		printf("\n5\n(18-25)        %d     ", *(auxPreguntas->Respuestas+6+4)); imprimirNumero(*(auxPreguntas->Respuestas+6+4));
		printf("(26-35)        %d     ", *(auxPreguntas->Respuestas+6*2+4)); imprimirNumero(*(auxPreguntas->Respuestas+6*2+4));
		printf("(36-45)        %d     ", *(auxPreguntas->Respuestas+6*3+4)); imprimirNumero(*(auxPreguntas->Respuestas+6*3+4));
		printf("(46-65)        %d     ", *(auxPreguntas->Respuestas+6*4+4)); imprimirNumero(*(auxPreguntas->Respuestas+6*4+4));
		printf("(66-120)       %d     ", *(auxPreguntas->Respuestas+6*5+4)); imprimirNumero(*(auxPreguntas->Respuestas+6*5+4));

		printf("%s\n",auxPreguntas->Encabezado);
		printf("\n6\n(18-25)        %d     ", *(auxPreguntas->Respuestas+6+5)); imprimirNumero(*(auxPreguntas->Respuestas+6+5));
		printf("(26-35)        %d     ", *(auxPreguntas->Respuestas+6*2+5)); imprimirNumero(*(auxPreguntas->Respuestas+6*2+5));
		printf("(36-45)        %d     ", *(auxPreguntas->Respuestas+6*3+5)); imprimirNumero(*(auxPreguntas->Respuestas+6*3+5));
		printf("(46-65)        %d     ", *(auxPreguntas->Respuestas+6*4+5)); imprimirNumero(*(auxPreguntas->Respuestas+6*4+5));
		printf("(66-120)       %d     ", *(auxPreguntas->Respuestas+6*5+5)); imprimirNumero(*(auxPreguntas->Respuestas+6*5+5));


	}
}

void imprimirNumero(int n){
	int i;
	for (i=0;i<n;i++)
		printf("*");
	printf("\n");
}

void liberarMemoria(Persona *pe,Cuestionario *c){
	Persona* auxPersona;
	for (auxPersona = pe; auxPersona < pe + TOTAL_ENCUESTAS; ++auxPersona){
		free(auxPersona->Nombre);
	}
	free(pe);
	Pregunta *auxPregunta;
	for (auxPregunta = c->preguntas; auxPregunta < c->preguntas + TOTAL_PREGUNTAS; ++ auxPregunta){
		free(auxPregunta->Respuestas);
		free(auxPregunta->Encabezado);
		char **auxOpciones;
		for (auxOpciones = auxPregunta->Opciones; auxOpciones < auxPregunta->Opciones + TOTAL_OPCIONES; ++auxOpciones){
			free(*(auxOpciones));
		}
		free(auxPregunta->Opciones);
	}
	free(c->preguntas);
	free(c);

}

