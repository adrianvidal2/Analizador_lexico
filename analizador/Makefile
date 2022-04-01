#Opcións de compilación (Warnings all)
CC=gcc -Wall

#Carpeta das cabeceiras
HEADER_FILES_DIR=.

#Opcións de compilación, indica onde están os arquivos .h
INCLUDES=-I $(HEADER_FILES_DIR)

#nome do executable
OUTPUT=compiladorGO

#fontes
SOURCES= main.c analizadorLexico.c analizadorSintactico.c sistemaEntrada.c abb.c taboaSimbolos.c xestionErros.c

#arquivos .h
LIB_HEADERS=$(HEADER_FILES_DIR)/definicions.h analizadorLexico.h analizadorSintactico.h sistemaEntrada.h abb.h taboaSimbolos.h erros.h xestionErros.h

#arquivos .o, un análogo a cada .c
OBJS= $(SOURCES:.c=.o)

#regla do executable
$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LIBS)

#regla que xera os .o cando é necesario.
#$@ será o nome dos arquivos xerados coa regla
#$< nome dos arquivos .c
%.o: %.c $(LIB_HEADEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)

#eliminación do executable (prerrequisito: clean)
cleanall: clean
	rm -f $(OUTPUT)

#eliminación dos arquivos .o e backups
clean:
	rm -f *.o *~
