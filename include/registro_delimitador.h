#ifndef _REGISTRO_DELIMITADOR_H_
#define _REGISTRO_DELIMITADOR_H_

#include <companhias.h>

void escreverCompanhiaDelimitador(FILE*,Companhia*);
Companhia *buscarNumRegDelimitador(FILE *,int);
Companhia *lerCompanhiaDelimitador(FILE *);
Companhia **buscarCampoDelimitador(FILE *, Campo, char *, int *);
Companhia **lerTodosDelimitador(FILE *, int*);

#endif