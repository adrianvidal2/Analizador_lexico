# Analizador léxico
[English](README.en.md)
Implementación de un **analizador léxico** que se encaragará de le devolución de los componentes léxicos que aparecen en el archivo *concurrentSum.go*. El analizador se encuentra escrito en el lenguaje **C**.

Esta práctica ha sido realizada para la asignatura de *Compiladores e Intérpretes*, el guión de la práctica se puede ver [aquí](https://github.com/adrianvidal2/Analizador_lexico/blob/main/guion).
## IDE utilizado
Eclipse
## Ficheros disponibles
- **main.c**: código de ejecución del programa.
- **tablaSimbolos.h** y **tablaSimbolos.c**: cabecera e implementación de lo que será la tabla de símbolos que contendrá las palabras clave del lenguaje e identificadores encontrados durante el análisis.
- **abb.h** y **abb.c**: cabecera e implementación del árbol binario de búsqueda que será la estructura que representa la tabla de símbolos.
- **analizadorSintactico.h** y **analizadorSintactico.c**: cabecera e implementación del analizador sintáctico, este se encarga de la petición de los componentes léxicos y de su impresión por pantalla.
- **analizadorLexico.h** y **analizadorLexico.c**: cabecera e implementación del analizador léxico, encargado de la formación de los lexemas y la devolución al analizador sintáctico junto con el código correspondiente.
- **sistemaEntrada.h** y **sistemaEntrada.c**: cabecera e implementación que permitirá la lectura del archivo, así como la ocupación de los centinelas.
- **xestionErros.h** y **xestionErros.c": cabecera e implementación de los diferentes tipos de errores.
- **erros.h**: definición de los tipos de errores que se pueden presentar.
- **definicions.h**: definición de constantes para los diferentes tipos de componentes léxicos que se pueden presentar.
- **concurrentSum.go**: fichero con una implementación básica de programa escrito en el lenguaje *GO* y que se desea analizar.

## Compilación y ejecución
Para la compilación de los ficheros mencionados anteriormente se proporciona un *Makefile*. Los ficheros se deben encontrar todos en el mismo lugar, exceptuando el fichero con el código a analizar, que puede estar en el lugar que se desee.
```
cd <carpeta-implementacion>
make
```

 El nombre del ejecutable generado será *compiladorGO*. Para la ejecución se deberá introducir el nombre del fichero que se quiere analizar, como argumento:
 
 ```
./compiladorGO [archivo]
```
Si el archivo no se encuentra en la misma carpeta se debe indicar la ruta del directorio en el cual se encuentra.
## Mejoras a considerar
- Ampliación del analizador léxico para el reconocimiento de más componentes léxicos.
- Cambio en la estructura de la tabla de datos, pues en este caso, no se tiene en cuenta el equilibrio.
## A tener en cuenta
Solo se han realizado pruebas en distintas distribuciones de Linux. Añadir que aunque el analizador no este completo, puede reconocer componente léxicos fuera del archivo utilizado para la práctica.
## Autor
[Adrián Vidal Lorenzo](https://github.com/adrianvidal2)

