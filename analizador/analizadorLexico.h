#ifndef ANALIZADORLEXICO_ANALIZADORLEXICO_H_

typedef struct{
	char *nome_lexema;
	int codigo;
} compLexico;


/*Función encargada da lectura caracter a caracter do arquivo de entrada*/
int seguinte_comp_lexico(compLexico *lexema);

#endif /* ANALIZADORLEXICO_ANALIZADORLEXICO_H_ */
