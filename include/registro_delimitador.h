#ifndef _REGISTRO_DELIMITADOR_H_
#define _REGISTRO_DELIMITADOR_H_

#include <companhias.h>

#define DELIM_FIM_CAMPO '|'
#define DELIM_FIM_REG '#'

char *lerAteDelimitadorRegistro(FILE *in, char *delimitador);
void escreverCompanhiaDelimitador(FILE*,Companhia*);
Companhia** lerTodosDelimitador(FILE *in, int* n_regs);
Companhia **buscarCampoDelimitador(FILE *in, Campo campo, char *query, int *n_regs);
Companhia *lerCompanhiaDelimitador(FILE *);
Companhia *buscarNomeDelimitador(FILE *,char *);
Companhia *buscarNumRegDelimitador(FILE *,int);
int possuiCampoProcurado(Companhia *, Campo, char *);

#endif