/*
Autores:

Gustavo Sutter - 9763193
Matheus Gomes - 9779270
Guilherme Montemovo - 9779461
Joice Aurino - 8530851

*/

#ifndef _REGISTRO_DELIMITADOR_H_
#define _REGISTRO_DELIMITADOR_H_

#include <companhias.h>

int escreverCompanhia(char*,Companhia*);
void imprimirTodos(FILE *);
Companhia *buscarNumRegDelimitador(FILE *,int);
Companhia *lerCompanhia(FILE *);
Companhia **buscarCampoDelimitador(FILE *, Campo, char *, int *);
void removerRegistro(FILE*,int);

#endif