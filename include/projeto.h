#ifndef _PROJETO_H_
#define _PROJETO_H_

#include <indice.h>
#include <companhias.h>

typedef struct Projeto Projeto;

struct Projeto{
	char *nome_projeto;
	char *nome_dados_ff;
	char *nome_dados_bf;
	char *nome_dados_wf;
	char *nome_idx_bf;
	char *nome_idx_ff;
	char *nome_idx_wf;
	Indice *best_fit;
	Indice *worst_fit;
	Indice *first_fit;
	unsigned char alterado;
};

/* Dado um nome um projeto é iniciado, ou recuperado do disco */
Projeto *iniciarProjeto(char *);
/* Insere as companhias de um arquivo csv nos arquivos dados e nos indices */
void inserirDoCSV(Projeto *, char *);
/* Remove uma companhia dos arquivos de dados e dos indices*/
void removerCompanhia(Projeto *, char *);
/* Imprime em formato de tabela os indices */
void imprimirIndices(Projeto *);
/* Insere uma companhia nos arquivos de dados e nos indices */
void inserirCompanhiaIndividual(Projeto *, Companhia *);
/* Salva os indices no disco */
void salvarIndices(Projeto*);
/* Imprime as listas de espaços para reuso */
void imprimirListas(Projeto *);
/* Libera as estruturas do projeto da memoria*/
void freeProjeto(Projeto*);


#endif