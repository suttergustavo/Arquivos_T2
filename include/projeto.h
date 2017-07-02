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

Projeto *iniciarProjeto(char *);
void inserirDoCSV(Projeto *, char *);
void removerCompanhia(Projeto *, char *);
void imprimirIndices(Projeto *);
void inserirCompanhiaIndividual(Projeto *, Companhia *);

#endif