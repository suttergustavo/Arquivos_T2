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
#include <indice.h>

typedef enum {
	FIRST_FIT,
	BEST_FIT,
	WORST_FIT
}Estrategia;


int escreverCompanhia(char*,Companhia*);
void imprimirTodos(char *, Indice*);
Companhia *lerCompanhia(char *, int);
void removerRegistro(char*,int,Estrategia);
void imprimirListaRemovidos(char *,char *);

#endif