#include "analizadorLexico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inicializar() {
	/*Creation and initialization of the token*/
	compLexico *lex;
	lex = (compLexico*) malloc(sizeof(compLexico));
	lex->nome_lexema = NULL;
	lex->codigo = -1;

	//Impression of the different lexical components
	while (seguinte_comp_lexico(lex)) {
		printf("<%d, %s>\n", lex->codigo, lex->nome_lexema);
	}

	//memory release
	if (lex->nome_lexema != NULL) {
		free(lex->nome_lexema);
		lex->nome_lexema = NULL;
	}
	free(lex);
	lex = NULL;
}
