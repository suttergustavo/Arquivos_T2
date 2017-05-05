#ifndef _REGISTRO_NUM_FIXO_CAMPOS_H_
#define _REGISTRO_NUM_FIXO_CAMPOS_H_

#include <companhias.h>

#define DELIM_FIM_CAMPO '|'

void escreverCompanhiaNumFixo(FILE*,Companhia*);
Companhia *lerCompanhiaNumFixo(FILE *);
Companhia **buscarCampoNumFixo(FILE *,Campo , char *,int *);
Companhia *buscarNumRegNumFixo(FILE *,int);


#endif