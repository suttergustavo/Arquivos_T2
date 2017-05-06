#ifndef _REGISTRO_INDICADOR_TAMANHO_H_
#define _REGISTRO_INDICADOR_TAMANHO_H_

#include <companhias.h>

void escreverCompanhiaTamReg(FILE*,Companhia*);
Companhia *buscarNumRegTamReg(FILE *,int);
Companhia *lerCompanhiaTamReg(FILE *);
Companhia **buscarCampoTamReg(FILE *in, Campo campo, char *query, int *n_regs);
Companhia **lerTodosTamReg(FILE *in, int* n_regs);

#endif