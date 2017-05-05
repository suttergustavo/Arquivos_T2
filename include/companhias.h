#ifndef _COMPANHIAS_H_
#define _COMPANHIAS_H_

typedef struct Companhia Companhia;

//TAMANHO DOS CAMPO DE TAMANHO FIXO
#define TAMANHO_CNPJ 19
#define TAMANHO_DATA 11


typedef enum{
	CNPJ,
	NOME_SOCIAL,
	NOME_FANTASIA,
	DATA_REGISTRO,
	DATA_CANCELAMENTO,
	MOTIVO_CANCELAMENTO,
	NOME_EMPRESA,
	CNPJ_AUDITORIA
}Campo;

struct Companhia{
	char *cnpj;
	char *nome_social;
	char *nome_fantasia;
	char *data_registro;
	char *data_cancelamento;
	char *motivo_cancelamento;
	char *nome_empresa;
	char *cnpj_auditoria;
};


Companhia **lerCSVCompleto(char *,int *);
void destruirCompanhia(Companhia*);
void imprimirCompanhia(Companhia*);
Companhia **lerTodosNumFixo(FILE *,int*);
Companhia *criarCompanhia();

#endif