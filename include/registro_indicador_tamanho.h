#ifndef _REGISTRO_INDICADOR_TAMANHO_H_
#define _REGISTRO_INDICADOR_TAMANHO_H_

#include <companhias.h>

#define DELIM_FIM_CAMPO '|'

void escreverCompanhiaTamReg(FILE*,Companhia*);
Companhia **lerTodosTamReg(FILE *in, int* n_regs);
Companhia **buscarCampoTamReg(FILE *in, Campo campo, char *query, int *n_regs);
Companhia *lerCompanhiaTamReg(FILE *);
Companhia *buscarNumRegTamReg(FILE *,int);

#endif