/*
Autores:

Gustavo Sutter - 9763193
Matheus Gomes - 9779270
Guilherme Montemovo - 9779461
Joice Aurino - 8530851

*/

#ifndef _COMPANHIAS_H_
#define _COMPANHIAS_H_

typedef struct Companhia Companhia;

//TAMANHO DOS CAMPO DE TAMANHO FIXO
#define TAMANHO_CNPJ 19
#define TAMANHO_DATA 11

//delimitadores de fim de campo e de arquivo
#define DELIM_FIM_CAMPO '|'
#define DELIM_FIM_REG '#'

#define FLAG_REMOVIDO '*'

//estrutura companhia, com os campos especificados
struct Companhia{
	char *cnpj;
	char *cnpj_auditoria;
	char *data_registro;
	char *data_cancelamento;
	char *nome_social;
	char *nome_fantasia;
	char *motivo_cancelamento;
	char *nome_empresa;
};

//tipo que define os campos da estrutura companhia
typedef enum{
	CNPJ = 1,
	CNPJ_AUDITORIA,
	DATA_REGISTRO,
	DATA_CANCELAMENTO,
	NOME_SOCIAL,
	NOME_FANTASIA,
	MOTIVO_CANCELAMENTO,
	NOME_EMPRESA
}Campo;


/* Aloca uma nova estrutura companhia */
Companhia *criarCompanhia(int);
/* Retorna um determinado campo de uma companhia */
char *getCampoCompanhia(Companhia *,Campo);
/* Recebe um nome de arquivo CSV e retorna todos os registros contidos nele */
Companhia **lerCSVCompleto(char *,int *);
/* Desaloca um estrutura companhia da memoria */
void destruirCompanhia(Companhia*);
/* Recebe uma estrutura que contem uma companhia e a imprime */
void imprimirCompanhia(Companhia*);
/* Checa se o uma compania tem um campo igual a uma query */
int possuiCampoProcurado(Companhia *, Campo, char *);
/* Retorna o tamanho da companhia em bytes(leva em consideração os delimitadores que serão inseridos)*/
int getTamanhoCompanhia(Companhia *);

Companhia *lerCompanhiaCSV(FILE *);
#endif