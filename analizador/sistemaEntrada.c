#include "sistemaEntrada.h"
#include "xestionErros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 64			//Size of each sentinel, multiple of the allocation unit (4096)
#define PRIMEIRO 0		//Position indicator on the first sentinel
#define SEGUNDO 1		//Position indicator on the second sentinel

typedef struct {
	char centA[N + 1];	//First sentinel
	char centB[N + 1];	//Second sentinel
	int inicio;			//Pointer that indicates the beginning of a lexeme
	int dianteiro;		//Pointer who will advance on the sentinels
	int turno;//Variable that allows knowing in which sentinel the reading is being performed
} buffer;

/*GLOBAL VARIABLES*/
FILE *arquivoEntrada;		//input file
int pos_lectura = 0;		//Current position in the file
buffer buf;
int sobrepasa = 0;			//Flag to know if the lexeme is larger than allowed
int retroceso = 1;			//Flag to know if I can load the next block

/*
 * Buffer initialization
 * */
void crearBuffer() {
	buf.inicio = 0;
	buf.dianteiro = 0;
	buf.turno = PRIMEIRO;
}

/*
 * Function in charge of opening the file, initializing the buffer and reading the first N characters stored in the first sentinel.
 * */
void apertura_arquivo(char *arquivo) {
	//Opening the file with error checking
	if ((arquivoEntrada = fopen(arquivo, "r")) != NULL) {
		crearBuffer();
		fseek(arquivoEntrada, pos_lectura, SEEK_SET);
		int caracteres_lidos = fread(buf.centA, sizeof(char), N,
				arquivoEntrada); //The first sentinel is filled
		buf.centA[caracteres_lidos] = EOF; //EOF is inserted in the last position
		if (!ferror(arquivoEntrada)) { //Error checking
			fflush(arquivoEntrada);
			pos_lectura = ftell(arquivoEntrada);//Storing the reading position in the file
		} else {
			erroArquivo(2);
		}

	} else
		erroArquivo(1);
}

/*File closure*/
void peche_arquivo() {
	fclose(arquivoEntrada);
}

/*
 * Private function in charge of the occupation of the next sentinel
 * Using the number of characters read, the EOF is placed in the last position.
 * */
void _ocupar_centinela() {
	int caracteres_lidos = 0;
	if (buf.turno == PRIMEIRO) {
		fseek(arquivoEntrada, pos_lectura, SEEK_SET);
		caracteres_lidos = fread(buf.centA, sizeof(char), N, arquivoEntrada);
		buf.centA[caracteres_lidos] = EOF;
		if (!ferror(arquivoEntrada)) {
			fflush(arquivoEntrada);
			pos_lectura = ftell(arquivoEntrada);
			buf.dianteiro = 0;
			if (sobrepasa)//If the lexeme is too long, we place the start pointer in the next block
				buf.inicio = N;
		} else {
			erroArquivo(2);
		}

	} else if (buf.turno == SEGUNDO) {
		fseek(arquivoEntrada, pos_lectura, SEEK_SET);
		caracteres_lidos = fread(buf.centB, sizeof(char), N, arquivoEntrada);
		buf.centB[caracteres_lidos] = EOF;
		if (!ferror(arquivoEntrada)) {
			fflush(arquivoEntrada);
			pos_lectura = ftell(arquivoEntrada);
			if (sobrepasa)
				buf.inicio = 0;
		} else {
			erroArquivo(2);
		}
	}
}

/*
 * Function in charge of returning the next character stored in the corresponding sentinel. If this character is EOF,
 * it should be checked whether it is end of file or end of sentinel.
 *
 * return caracter: sentinel current character
 * */
char seguinte_caracter() {
	char caracter_leido;
	if (buf.turno == PRIMEIRO) {
		//Check if the read character is the end of file or another type of character
		if ((caracter_leido = buf.centA[buf.dianteiro]) == EOF) {

			//In case of finding the EOF symbol, it is checked if it is the end of file or block
			if (!feof(arquivoEntrada)) {
				//In case of finishing the block, the following must be filled in
				buf.turno = SEGUNDO;
				//It is checked that the next block can be loaded, since it was possible to go back to the first one or accept a lexeme
				if (retroceso) {
					//If start is in the next block, it means that the lexeme exceeds the allowed size
					if (buf.inicio > N) {
						sobrepasa = 1;
					}
					_ocupar_centinela();
				} else {
					retroceso = 0;
				}
				caracter_leido = seguinte_caracter();//The function is called again to return the character

			} else {
				buf.dianteiro++;
			}
		} else {
			buf.dianteiro++;
		}
	} else if (buf.turno == SEGUNDO) {

		if ((caracter_leido = buf.centB[buf.dianteiro - N]) == EOF) {

			if (!feof(arquivoEntrada)) {

				buf.turno = PRIMEIRO;

				if (retroceso) {
					if (buf.inicio <= N) {
						sobrepasa = 1;
					}
					_ocupar_centinela();
				} else {
					retroceso = 0;
				}
				caracter_leido = seguinte_caracter();
			} else
				buf.dianteiro++;
		} else {
			buf.dianteiro++;
		}
	}

	return caracter_leido;
}

/*
 * Calculation to know the size of the lexeme
 * */
int _tam_lexema() {
	if (buf.inicio < N && buf.dianteiro > N) {
		return buf.dianteiro - buf.inicio;
	} else if (buf.inicio > buf.dianteiro){
		return (N * 2) - buf.inicio + buf.dianteiro;
	}
	return buf.dianteiro - buf.inicio;
}

/*
 * Function that will make the forward pointer go back to the corresponding position when a character is found that determines the end of the lexeme.
 * Take into account those cases in which the recoil causes a change of sentinel
 * */
void devolver_caracter() {
	if (buf.turno == PRIMEIRO) {
		if (buf.dianteiro == 0) {
			buf.turno = SEGUNDO;
			buf.dianteiro = N * 2 - 1;
			retroceso = 0;
		} else {
			buf.dianteiro--;
		}
	} else if (buf.turno == SEGUNDO) {
		if (buf.dianteiro == N) {
			buf.turno = PRIMEIRO;
			buf.dianteiro = N - 1;
			retroceso = 0;
		} else {
			buf.dianteiro--;
		}
	}
}

/*
 * Functions that retrieve the lexeme and return it
 *
 * In case the size exceeds the allowed one, the last part of the lexeme will be returned
 * */

void _recuperar(char **lexema, int tam, int avanza) {

	if (!avanza) {
		*lexema = (char*) malloc(tam + 1 * sizeof(char));//memory reserve for the lexeme
		if (buf.inicio < N && buf.dianteiro >= N) {	//Differents sentinels
			strncpy(*lexema, buf.centA + buf.inicio, N - buf.inicio);
			strncpy(*lexema + N - buf.inicio, buf.centB, buf.dianteiro - N);
		} else if (buf.inicio > buf.dianteiro) {//Initial in second sentinel and forward in first sentinel.
			strncpy(*lexema, buf.centB + buf.inicio - N, (N * 2) - buf.inicio);
			strncpy(*lexema + (N * 2) - buf.inicio, buf.centA, buf.dianteiro);

		} else {
			if (buf.turno == PRIMEIRO) {
				strncpy(*lexema, buf.centA + buf.inicio,
						buf.dianteiro - buf.inicio);

			} else if (buf.turno == SEGUNDO) {
				strncpy(*lexema, buf.centB + buf.inicio - N,
						buf.dianteiro - buf.inicio);
			}
		}
		*(*lexema + tam) = '\0';
	} else {//In this case, the length of the lexeme can only be N characters, which is the maximum allowed, returning the last N characters
		*lexema = (char*) malloc(N + 1 * sizeof(char));
		int avanzar = tam - N;
		if (buf.inicio < N && buf.dianteiro >= N) {
			strncpy(*lexema, buf.centA + buf.inicio + avanzar,
			N - buf.inicio - avanzar);
			strncpy(*lexema + N - buf.inicio - avanzar, buf.centB,
					buf.dianteiro - N);
		} else if (buf.inicio > buf.dianteiro) {
			strncpy(*lexema, buf.centB + buf.inicio - N + avanzar,
					(N * 2) - buf.inicio - avanzar);
			strncpy(*lexema + (N * 2) - buf.inicio - avanzar, buf.centA,
					buf.dianteiro);

		}
		*(*lexema + N) = '\0';
	}

}
void recuperar_lexema(char **lexema) {

	int tam = _tam_lexema();
	if (!sobrepasa) {

		if (tam <= N) {
			_recuperar(lexema, tam, 0);
		} else {
			erroLexico(1, "");
			_recuperar(lexema, tam, 1);
		}
	} else {
		erroLexico(1, "");
		_recuperar(lexema, tam, 0);
	}
}

/*Función que colocará o punteiro do inicio de lexema na posición correspondente o seguinte lexema*/
void aceptar_lexema() {
	buf.inicio = buf.dianteiro;
	sobrepasa = 0;	//Desactivamos o flag de lexema moi grande
}

