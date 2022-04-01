#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "definicions.h"

/*Representation of the symbol table*/
abb arbore;

/*Keywords structure*/
typedef struct {
	char *lexema;
	int rexistro;
} palabras_reservadas;

//Initial keywords
palabras_reservadas estructuras[] = { { "package", PACKAGE },
		{ "import", IMPORT }, { "func", FUNC }, { "chan", CHAN },
		{ "var", VAR }, { "range", RANGE }, { "go", GO }, { "for", FOR } };

//Creating table and inserting keywords in the table
void inicializarTaboa() {
	int nElems = sizeof(estructuras) / sizeof(estructuras[0]);
	crear(&arbore);

	for (int i = 0; i < nElems; i++) {
		insertar(&arbore, estructuras[i].lexema, estructuras[i].rexistro);

	}
}

//Destruction of the table
void destruirTaboa() {
	destruir(&arbore);
}

//Print table
void imprimirTaboa() {
	imprimir_tabla(&arbore);
}

//Search and insertion of lexeme
int buscar(char *lexema) {
	return buscar_lexema(&arbore, lexema);
}
