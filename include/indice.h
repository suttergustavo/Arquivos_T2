#ifndef _INDICE_H_
#define _INDICE_H_

typedef struct{
	char *cnpj;
	int offset;
}RegIndice;

typedef struct{
	RegIndice **indice;
	int size;
}Indice;

typedef enum {
	INVALIDO,
	VALIDO
}Validade;

void salvarIndice(FILE *, Indice *);
Indice *carregarIndice(FILE *);
int buscarIndice(Indice *, char *);
void validadeIndice(FILE *, Validade);
void inserirIndice(Indice *,char *,int );
int removerIndice(Indice *, char *);
void imprimirIndice(FILE *);
void destruirIndice(RegIndice *);
void destruirIndiceCompleto(Indice *);
Indice *criarIndice();

#endif