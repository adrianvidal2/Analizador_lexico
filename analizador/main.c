#include "taboaSimbolos.h"
#include "analizadorSintactico.h"
#include <stdio.h>
#include <stdlib.h>
#include "sistemaEntrada.h"

int main(int argc, char **argv) {
	if (argc == 2) {
		apertura_arquivo(argv[1]);
		inicializarTaboa();
		printf("======== TABOA SÍMBOLOS =========\n");
		imprimirTaboa();
		printf("=================================\n\n");
		inicializar();
		peche_arquivo();
		printf("\n\n======== TABOA SÍMBOLOS =========\n");
		imprimirTaboa();
		printf("=================================\n");
		destruirTaboa();
	} else {
		printf("Número de argumentos inválido: ./executable [arquivo]\n");
	}
}
