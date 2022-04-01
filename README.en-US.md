# Lexical analyzer


[Español](REEDME.md)
[English](REEDME.en.md)

Implementation of a **lexical analyzer** that will handle the return of the lexical components that appear in the *concurrentSum.go* file. The parser is written in the **C** language.

This practice has been carried out for the subject of *Compilers and Interpreters*, the script of the practice can be seen [here](https://github.com/adrianvidal2/Analizador_lexico/blob/main/guion). The script is written in Spanish.

## IDE
Eclipse

## Available files
- **main.c**: program execution code.
- **tablaSimbolos.h** y **tablaSimbolos.c**: header and implementation of what will be the symbol table that will contain the language keywords and identifiers found during the analysis.
- **abb.h** y **abb.c**: header and implementation of the binary search tree that will be the structure that represents the symbol table.
- **analizadorSintactico.h** y **analizadorSintactico.c**: header and implementation of the parser, this is in charge of requesting the tokens and printing them on the screen.
- **analizadorLexico.h** y **analizadorLexico.c**: header and implementation of the lexical analyzer, in charge of the formation of the lexemes and the return to the parser together with the corresponding code.
- **sistemaEntrada.h** y **sistemaEntrada.c**: header and implementation that will allow the reading of the file, as well as the occupation of the sentinels..
- **xestionErros.h** y **xestionErros.c": header and implementation of the different types of errors.
- **erros.h**: definition of the types of errors that can occur.
- **definicions.h**: definition of constants for the different types of tokens that can occur.
- **concurrentSum.go**: file with a basic implementation of a program written in the *GO* language and that you want to analyze.


## Compilation and execution
For the compilation of the files mentioned above a *Makefile* is provided. The files must all be found in the same place, except for the file with the code to be analyzed, which can be in any desired place.


```
cd <carpeta-implementacion>
make
```

The name of the generated executable will be *GOcompiler*. For execution, the name of the file to be analyzed must be entered as an argument:

```
./compiladorGO [file]
```

If the file is not in the same folder, the path of the directory in which it is located must be indicated.


## Improvements to consider
- Extension of the lexical analyzer for the recognition of more lexical components.
- Change in the structure of the data table, because in this case, the balance is not taken into account.


## Notes
Only tested on different Linux distributions. Add that although the parser is not complete, it can recognize tokens outside the file used for practice.


## Author

[Adrián Vidal Lorenzo](https://github.com/adrianvidal2)

