/*
Autores:

Gustavo Sutter - 9763193
Matheus Gomes - 9779270
Guilherme Montemovo - 9779461
Joice Aurino - 8530851

*/

#ifndef _REGISTRO_INDICADOR_TAMANHO_H_
#define _REGISTRO_INDICADOR_TAMANHO_H_

#include <companhias.h>

void escreverCompanhiaTamReg(FILE*,Companhia*);
Companhia *buscarNumRegTamReg(FILE *,int);
Companhia *lerCompanhiaTamReg(FILE *);
Companhia **buscarCampoTamReg(FILE *in, Campo campo, char *query, int *n_regs);
Companhia **lerTodosTamReg(FILE *in, int* n_regs);

#endif