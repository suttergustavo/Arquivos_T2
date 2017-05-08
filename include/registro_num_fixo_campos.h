/*
Autores:

Gustavo Sutter - 9763193
Matheus Gomes - 9779270
Guilherme Montemovo - 9779461
Joice Aurino - 8530851

*/

#ifndef _REGISTRO_NUM_FIXO_CAMPOS_H_
#define _REGISTRO_NUM_FIXO_CAMPOS_H_

#include <companhias.h>

void escreverCompanhiaNumFixo(FILE*,Companhia*);
Companhia *buscarNumRegNumFixo(FILE *,int);
Companhia *lerCompanhiaNumFixo(FILE *);
Companhia **buscarCampoNumFixo(FILE *,Campo , char *,int *);
Companhia **lerTodosNumFixo(FILE *, int*);

#endif