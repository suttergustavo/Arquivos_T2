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

/* Tranfere para o arquivo no disco as informações nas estruturas na memoria */
void salvarIndice(char *, Indice *);
/* Tranfere um indice do disco para memoria, ou cria caso não exista*/
Indice *carregarIndice(char *, char *);
/* Seta o byte de validade no cabeçalho do arquivo */
void validadeIndice(char*, Validade);
/*Insere novo indice no vetor de indices*/
void inserirIndice(Indice *,char *,int );
/* Remove um indice e retorna offset do registro */
int removerIndice(Indice *, char *);
/* Desaloca o indice da memoria */
void destruirIndice(Indice *);
/* Aloca uma estrutura indice */
Indice *criarIndice();

#endif