#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "taboaSimbolos.h"
#include "definicions.h"
#include "sistemaEntrada.h"
#include "xestionErros.h"


/*	STRUCTURE OF THE LEXICAL COMPONENT
 *	char *nome_lexema: lexeme
 *	int codigo: lexeme code
 * */
typedef struct {
	char *nome_lexema;
	int codigo;
} compLexico;

/*Memory release of lexeme name*/
void liberarMemoria(compLexico *lexema) {
	if (lexema->nome_lexema != NULL) {
		free(lexema->nome_lexema);
		lexema->nome_lexema = NULL;
	}
	lexema->codigo = 0;
}

/*PRIVATE FUNCTIONS*/

/*Checking that numbers containing an exponent are well formed
 *Arguments:
 * 	int *malFormado: integer that allows to know if the lexeme contains an invalid character
 * 	char *c = pointer to current character.
 * */
void _comprobar_exponhente(int *malFormado, char *c) {
	*c = seguinte_caracter();
	if (*c == '+' || *c == '-') {
		*c = seguinte_caracter();
		if (!isdigit(*c)) {
			devolver_caracter();
			*malFormado = 0;
		}

	} else if (!isdigit(*c)) {
		devolver_caracter();
		*malFormado = 0;
	}

	if (*malFormado)
		*c = seguinte_caracter();
}

/* Function responsible for the recovery of the lexeme of the input system
 * Arguments:
 * 	compLexico *lexema: lexical component structure
 * 	int codigo: lexeme code.
 * 	int devolve: indicates if the character should be returned (for cases of operators recognized as ascii)
 *
 * 	Return:
 * 		1: no error
 * 		0: error
 * */
void _recuperar_lexema(compLexico *lexema, int codigo, int devolve) {
	liberarMemoria(lexema);
	if (devolve)
		devolver_caracter();
	recuperar_lexema(&(lexema->nome_lexema));
		if (codigo == ID) {
			codigo = buscar(lexema->nome_lexema);
		}
		lexema->codigo = codigo;
		aceptar_lexema();
}

/*PUBLIC FUNCTION*/

/* Function that will continuously request characters until a lexeme can be formed
 * Arguments:
 * 	compLexico *lexema: lexical component structure
 *
 *Return:
 *	int: lexeme code
 * */
int seguinte_comp_lexico(compLexico *lexema) {
	int aceptar = 1;
	int estado = 0;		//state of the automaton
	int malFormado = 1;	//Lexeme error
	int exponhente = 1;	//It allows to ensure that the exponent does not appear more than once in a number, so that, given the case, it is recognized as a different one.
	char c;
	while (aceptar) {
		switch (estado) {
		case 0:	//State 0 of all automata
			c = seguinte_caracter();

			if (isblank(c) || c == '\n' || c == '\r' || c == ',' || c == ';') {	//The lexeme is accepted, except in two cases
				if (c == ',' || c == ';') {
					_recuperar_lexema(lexema, c, 0);
					estado = 11;
					break;
				}
				aceptar_lexema();
			} else if (isalpha(c) || c == '_') {//First state of the alphanumeric automaton
				estado = 1;
			} else if (isdigit(c)) {			//First state of the numerical automaton
				estado = 2;
			} else if (c == '"') {				//String automaton
				estado = 3;
			} else if (c == '+' || c == '-' || c == '*' || c == '%' || c == ':'
					|| c == '=' || c == '/') {		//Operator automaton
				estado = 4;
			} else if (c == '[' || c == ']' || c == '(' || c == ')' || c == '{'
					|| c == '}') {//Operator automata that will not have an = after
				estado = 5;
			} else if (c == '<') {					//Operator <
				estado = 8;
			} else if (c == '.') {//Automata that will recognize if it is a single point or a float number
				estado = 7;
			} else if (c == EOF) {//End of file.
				liberarMemoria(lexema);
				aceptar = 0;
			}
			break;
		case 1:
			/*			ALPHANUMERIC AUTOMATON
			 * Characters are requested until an element other than letters, numbers or _ is found.
			 * */
			c = seguinte_caracter();
			while (isalpha(c) || isdigit(c) || c == '_') {
				c = seguinte_caracter();
			}

			_recuperar_lexema(lexema, ID, 1);
			estado = 11;
			break;
		case 2:
			/*INTEGER NUMERICAL AUTOMATON*/
			if (c == '0') {
				/*If the number is a 0, there are 3 possibilities;
				 * 		- Binary: comezan  start with 0b or 0B
				 * 		- hexadecimail: start with 0, 0x or 0X
				 * 		- octal: start with 0o or 0O
				 * */
				c = seguinte_caracter();

				if (c == 'b' || c == 'B') {	//Binary automaton
				//Characters are requested until a binary digit is found or after a _ there is no binary digit.
					c = seguinte_caracter();
					while (c == '_' || c == 48 || c == 50) {
						if (c == '_') {
							c = seguinte_caracter();
							if (c != 48 || c != 50) {
								devolver_caracter();
								break;
							}
						}
					}

				} else if (c == 'x' || c == 'X' || c == '_') {//Hexadecimal automaton
				//If a 0_ is arrived, the following number is checked, since if it does not correspond to a hexadecimal number it will be an integer followed by an identifier.
					if (c == '_') {
						c = seguinte_caracter();
						if (!isxdigit(c)) {
							devolver_caracter();
							_recuperar_lexema(lexema, ENTEIRO, 1);
							estado = 0;
							break;
						}
					}
					//Characters are requested until it is not a hexadecimal number or after _ there is not a hexadecimal number
					c = seguinte_caracter();
					while (isxdigit(c) || c == '_') {
						if (c == '_') {
							c = seguinte_caracter();
							if (!isxdigit(c)) {
								devolver_caracter();
								break;
							}
						}
						c = seguinte_caracter();
					}

				} else if (c == 'o' || c == 'O') {	//Octal automaton
				//Characters are requested until it is not a number between 0 and 7 or there is an octal number after a _
					c = seguinte_caracter();
					while ((c >= 48 && c <= 55) || c == '_') {
						if (c == '_') {
							c = seguinte_caracter();
							if (c < 48 || c > 55) {
								devolver_caracter();
								break;
							}
						}
						c = seguinte_caracter();
					}
				} else if (c == '.' || c == 'e' || c == 'E') {//Decimal number
					estado = 6;
				} else if (c >= 48 && c <= 55) {//Octal numbers with format 01234567
					c = seguinte_caracter();
					while ((c >= 48 && c <= 55) || c == '_' || c == '.') {
						if (c == '_') {
							c = seguinte_caracter();
							if (c < 48 || c > 55) {
								devolver_caracter();
								break;
							}
						} else if (c == '.') {//Float numbers
							estado = 6;
							break;
						}
						c = seguinte_caracter();
					}
				}

			} else {	//Integers that do not start with 0
				c = seguinte_caracter();

				/*Characters are requested to find something other than a decimal digit, the following cases occur:
				 * 		- Float numbers:
				 * 				*an e or E after a number
				 * 				*an . after a number
				 * 		*/
				while (isdigit(c) || c == '_' || c == 'e' || c == 'E'
						|| c == '.') {
					if (c == '_') {
						c = seguinte_caracter();
						if (!isdigit(c)) {
							devolver_caracter();
							break;
						}
					} else if (c == 'e' || c == 'E' || c == '.') {
						estado = 6;
						break;
					}
					c = seguinte_caracter();
				}
			}

			//Float numbers
			if (estado == 6)
				continue;

			if (c == 'i') {	//If the automaton came out to read an i, it means that it is an imaginary
				_recuperar_lexema(lexema, IMAXINARIO, 0);
			} else {
				_recuperar_lexema(lexema, ENTEIRO, 1);
			}

			estado = 11;

			break;
		case 3:
			/*		STRIGNS AUTOMATON
			 * Character consumption until found "
			 * */
			c = seguinte_caracter();
			while (c != '"') {
				if (c == '\\')
					c = seguinte_caracter();
				c = seguinte_caracter();
			}

			_recuperar_lexema(lexema, STRING, 0);

			estado = 11;

			break;
		case 4:
			/*		OPERATOR AUTOMATON
			 * In case of not forming a valid operator, the ascii of said element will be returned
			 * */
			if (c == '+') {
				c = seguinte_caracter();
				switch (c) {
				case '+':
					c = seguinte_caracter();
					_recuperar_lexema(lexema, OP_MAS_MAS, 1);
					break;
				case '=':
					c = seguinte_caracter();
					_recuperar_lexema(lexema, OP_MAS_IGUAL, 1);
					break;
				default:
					_recuperar_lexema(lexema, '+', 1);
					break;
				}
			} else if (c == ':') {
				c = seguinte_caracter();
				if (c == '=') {
					_recuperar_lexema(lexema, OP_PUNTOS_IGUAL, 0);
				} else {
					_recuperar_lexema(lexema, ':', 1);
				}
			} else if (c == '=') {
				c = seguinte_caracter();
				if (c == '=') {
					_recuperar_lexema(lexema, OP_ASIGNACION, 0);
				} else {
					_recuperar_lexema(lexema, '=', 1);
				}
			} else if (c == '/') {
				c = seguinte_caracter();
				switch (c) {
				case '=':
					_recuperar_lexema(lexema, OP_BARRA_IGUAL, 0);
					break;
				case '*':
					/* COMMENTS AUTOMATON
					 * characters are consumed and accepted
					 * */
					c = seguinte_caracter();
					while (1) {
						if (c == '*') {
							c = seguinte_caracter();
							if (c == '/')
								break;
							devolver_caracter();
						}
						c = seguinte_caracter();
						aceptar_lexema();
					}
					aceptar_lexema();
					estado = 0;
					break;
				case '/':
					c = seguinte_caracter();
					aceptar_lexema();
					while (c != '\n' && c != '\r') {
						c = seguinte_caracter();
						aceptar_lexema();
					}
					aceptar_lexema();
					estado = 0;
					break;
				default:
					_recuperar_lexema(lexema, '/', 1);
					break;
				}
			} else if (c == '*') {
				_recuperar_lexema(lexema, '*', 0);
			} else if (c == '-') {
				_recuperar_lexema(lexema, '-', 0);

			}
			if (estado != 0)
				estado = 11;
			break;

		case 5:
			/*Operator automaton (returns ascii)*/
			_recuperar_lexema(lexema, c, 0);
			estado = 11;
			break;
		case 6:
			/*		FLOAT NUMBERS AUTOMATON		 */

			//Checking that it is a valid exponent otherwise there is a malformed lexeme error
			if (c == 'e' || c == 'E') {
				_comprobar_exponhente(&malFormado, &c);
				if (!malFormado) {
					break;
				}

				while (isdigit(c) || c == '_') {
					if (c == '_') {
						c = seguinte_caracter();
						if (!isdigit(c)) {
							devolver_caracter();
							break;
						}
					}
					c = seguinte_caracter();
				}

			} else if (c == '.') {
				c = seguinte_caracter();
				//It can be a number like 1.
				if (!isdigit(c) && c != 'e' && c != 'E') {
					if(c == 'i'){
						_recuperar_lexema(lexema, IMAXINARIO, 0);
						estado = 11;
						break;
					}
					_recuperar_lexema(lexema, FLOAT, 1);
					estado = 11;
					break;
				}
				//If an e or E arrives after the dot, check that the exponent is well formed and make sure that no other exponent is recognized.
				if (c == 'e' || c == 'E') {
					_comprobar_exponhente(&malFormado, &c);
					if (!malFormado)
						break;
					exponhente = 0;
				}

				//Digit request. An exponent may appear, so it must be verified that it is well formed and that it will not be recognized as such.
				while (isdigit(c) || c == '_' || c == 'e' || c == 'E') {
					if (c == '_') {
						c = seguinte_caracter();
						if (!isdigit(c)) {
							devolver_caracter();
							break;
						}
					} else if ((c == 'e' || c == 'E') && exponhente) {
						_comprobar_exponhente(&malFormado, &c);
						if (!malFormado)
							break;
						exponhente = 0;
						if (c == '_') {
							c = seguinte_caracter();
							if (!isdigit(c)) {
								devolver_caracter();
								break;
							}
						}
					} else if ((c == 'e' || c == 'E') && !exponhente) {
						break;
					}

					c = seguinte_caracter();
				}
			}

			if (c == 'i') {
				_recuperar_lexema(lexema, IMAXINARIO, 0);
			} else if (!malFormado) {
				_recuperar_lexema(lexema, -1, 0);
				erroLexico(2, lexema->nome_lexema);
				estado = 0;
			} else {
				_recuperar_lexema(lexema, FLOAT, 1);
			}

			if (estado != 0)
				estado = 11;
			break;
		case 7:
			//In this state it will be checked if it is a number or just a dot
			c = seguinte_caracter();
			if (!isdigit(c)) {
				_recuperar_lexema(lexema, '.', 1);
				estado = 11;
			} else {

				c = seguinte_caracter();
				while (isdigit(c) || c == '_' || c == 'i') {
					if (c == '_') {
						c = seguinte_caracter();
						if (!isdigit(c)) {
							devolver_caracter();
							malFormado = 0;
							break;
						}
					} else if (c == 'i') {
						break;
					}

					c = seguinte_caracter();
				}

				if (malFormado && (c == 'e' || c == 'E')) {	// If the lexeme is well formed and we find an exponent, we go to the float automaton
					estado = 6;
					continue;
				} else if (c == 'i') {
					_recuperar_lexema(lexema, IMAXINARIO, 0);
				} else if (!malFormado) {
					_recuperar_lexema(lexema, FLOAT, 1);
				} else {
					_recuperar_lexema(lexema, FLOAT, 1);
				}

				estado = 11;
			}

			break;
		case 8:
			//Sequence <-
			c = seguinte_caracter();
			if (c == '-') {
				_recuperar_lexema(lexema, OP_MENOS_GUION, 0);

			} else {
				_recuperar_lexema(lexema, '-', 1);
			}

			estado = 11;
			break;
		default:
			aceptar = 0;
		}

	}
	return lexema->codigo;
}

