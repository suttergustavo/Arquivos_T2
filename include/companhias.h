#ifndef _COMPANHIAS_H_
#define _COMPANHIAS_H_

typedef struct Companhia Companhia;

//TAMANHO DOS CAMPO DE TAMANHO FIXO
#define TAMANHO_CNPJ 19
#define TAMANHO_DATA 11

#define DELIM_FIM_CAMPO '|'
#define DELIM_FIM_REG '#'

typedef enum{
	CNPJ = 1,
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


char *getCampoCompanhia(Companhia *,Campo);
Companhia **lerCSVCompleto(char *,int *);
void destruirCompanhia(Companhia*);
void imprimirCompanhia(Companhia*);
Companhia **lerTodosNumFixo(FILE *,int*);
Companhia *criarCompanhia();
int possuiCampoProcurado(Companhia *, Campo, char *);

#endif