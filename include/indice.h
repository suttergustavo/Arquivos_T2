#ifndef _INDICE_H_
#define _INDICE_H_

typedef struct{
	char *cnpj;
	int offset;
}RegIndice;

void escreverIndice(FILE *,char *,int);
int buscarIndice(FILE *,char*);
RegIndice *lerIndice(FILE*);

#endif