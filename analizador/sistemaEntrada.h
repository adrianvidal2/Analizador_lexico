#ifndef MANEXOENTRADA_SISTEMAENTRADA_H_
/*FILE FUNCTIONS*/
void apertura_arquivo(char *arquivo);
void peche_arquivo();

/*LEXEME FUNCTIONS*/
char seguinte_caracter();
void devolver_caracter();
void recuperar_lexema(char **lexema);
void aceptar_lexema();

#endif /* MANEXOENTRADA_SISTEMAENTRADA_H_ */
