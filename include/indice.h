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

void salvarIndice(char *, Indice *);
Indice *recuperarIndice(char *);
Indice *carregarIndice(char *, char *);
int buscarIndice(Indice *, char *);
void validadeIndice(FILE *, Validade);
void inserirIndice(Indice *,char *,int );
int removerIndice(Indice *, char *);
void imprimirIndice(FILE *);
void destruirRegIndice(RegIndice *);
void destruirIndice(Indice *);
Indice *criarIndice();

#endif