#include "xestionErros.h"
#include <stdio.h>

//File erros
void erroArquivo(int codigo){
	switch(codigo){
	case 1:
		/*File cannot be opened*/
		printf("Non se pode abrir o arquivo\n");
		break;
	case 2:
		/*Error reading file*/
		printf("Erro na lectura do arquivo\n");
		break;
	}
}

//Lexeme erros
void erroLexico(int codigo, char *lexema) {
	switch (codigo) {
	case 1:
		/*Lexeme too large*/
		printf("O lexema supera o tamaño máximo permitido\n");
		break;
	case 2:
		/*Misformed lexema*/
		printf("Lexema mal formado %s\n", lexema);
		break;
	}
}
