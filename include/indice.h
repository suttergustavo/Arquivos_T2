#ifndef _INDICE_H_
#define _INDICE_H_

typedef struct{
	char *cnpj;
	int offset;
}RegIndice;

void criarIndice(FILE *, FILE *);
void escreverIndice(FILE *,char *,int);
int possuiIndiceProcurado(RegIndice *, char *);
void imprimirIndice(FILE *);
void destruirIndice(RegIndice *);
RegIndice *lerIndice(FILE*);

#endif