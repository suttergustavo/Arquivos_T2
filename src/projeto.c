#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <registro_delimitador.h>
#include <projeto.h>

/* Função usada simplesmente para concatenação a fim de criar nomes de arquivos*/ 
char *criarNomeArquivo(char *str1, char *str2){
	char *res = (char *) malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
	strcpy(res,str1);
	strcat(res,str2);
	return res;
}

/* Dado um nome um projeto é iniciado, ou recuperado do disco */
Projeto *iniciarProjeto(char *nome){
	Projeto *projeto = (Projeto*) malloc(sizeof(Projeto));

	
	//nome que do projeto
	projeto->nome_projeto = nome;

	//nome dos arquivos que o projeto manipulará
	projeto->nome_dados_ff = criarNomeArquivo(nome,"_dadosFF.dat");
	projeto->nome_dados_bf = criarNomeArquivo(nome,"_dadosBF.dat");
	projeto->nome_dados_wf = criarNomeArquivo(nome,"_dadosWF.dat");
	projeto->nome_idx_ff = criarNomeArquivo(nome,"_indiceFF.dat");
	projeto->nome_idx_bf = criarNomeArquivo(nome,"_indiceBF.dat");
	projeto->nome_idx_wf = criarNomeArquivo(nome,"_indiceWF.dat");
	
	//carrega(ou cria caso,não existam) os indices do projeto
	projeto->first_fit = carregarIndice(projeto->nome_idx_ff, projeto->nome_dados_ff);
	projeto->best_fit = carregarIndice(projeto->nome_idx_bf, projeto->nome_dados_bf);
	projeto->worst_fit = carregarIndice(projeto->nome_idx_wf, projeto->nome_dados_wf);

	return projeto;
}

void inserirDoCSV(Projeto *projeto, char *arquivo_csv){
	FILE *csv = fopen(arquivo_csv,"r");
	Companhia *companhia;
	int offset;

	if(csv == NULL){
		printf("Arquivo CSV não encontrado\n");
		return;
	}

	fseek(csv,0,SEEK_END);
	int size = (int) ftell(csv);	
	fseek(csv,0,SEEK_SET);
	
	if(projeto->alterado == 0){
		projeto->alterado = 1;
		validadeIndice(projeto->nome_idx_ff,INVALIDO);
		validadeIndice(projeto->nome_idx_bf,INVALIDO);
		validadeIndice(projeto->nome_idx_wf,INVALIDO);
	}

	while(size > (int) ftell(csv)){
		companhia = lerCompanhiaCSV(csv);

		offset = escreverCompanhia(projeto->nome_dados_ff,companhia,FIRST_FIT);
		inserirIndice(projeto->first_fit,companhia->cnpj,offset);

		offset = escreverCompanhia(projeto->nome_dados_bf,companhia,BEST_FIT);
		inserirIndice(projeto->best_fit,companhia->cnpj,offset);

		offset = escreverCompanhia(projeto->nome_dados_wf,companhia,WORST_FIT);
		inserirIndice(projeto->worst_fit,companhia->cnpj,offset);
	}
}

void removerCompanhia(Projeto *projeto, char *cnpj){
	int offset;

	offset = removerIndice(projeto->first_fit,cnpj);
	if(offset != -1) removerRegistro(projeto->nome_dados_ff,offset,FIRST_FIT);

	offset = removerIndice(projeto->best_fit,cnpj);
	if(offset != -1) removerRegistro(projeto->nome_dados_bf,offset,BEST_FIT);

	offset = removerIndice(projeto->worst_fit,cnpj);
	if(offset != -1) removerRegistro(projeto->nome_dados_wf,offset,WORST_FIT);
}

void inserirCompanhiaIndividual(Projeto *projeto, Companhia *companhia){
	int offset;

	offset = escreverCompanhia(projeto->nome_dados_ff,companhia,FIRST_FIT);
	inserirIndice(projeto->first_fit,companhia->cnpj,offset);

	offset = escreverCompanhia(projeto->nome_dados_bf,companhia,BEST_FIT);
	inserirIndice(projeto->best_fit,companhia->cnpj,offset);

	offset = escreverCompanhia(projeto->nome_dados_wf,companhia,WORST_FIT);
	inserirIndice(projeto->worst_fit,companhia->cnpj,offset);
}

void imprimirIndices(Projeto *projeto){
	for(int i=0;i<projeto->first_fit->size;i++){
		printf("%s\t\t%d\t%d\t%d\t\n",projeto->first_fit->indice[i]->cnpj,projeto->first_fit->indice[i]->offset,projeto->best_fit->indice[i]->offset,projeto->worst_fit->indice[i]->offset);
	}
	printf("\n");
}

void imprimirListas(Projeto *projeto){
	imprimirListaRemovidos(projeto->nome_dados_ff,"First Fit");
	imprimirListaRemovidos(projeto->nome_dados_bf,"Best Fit");
	imprimirListaRemovidos(projeto->nome_dados_wf,"Worst Fit");
}

void salvarIndices(Projeto *projeto){
	salvarIndice(projeto->nome_idx_ff,projeto->first_fit);
	salvarIndice(projeto->nome_idx_bf,projeto->best_fit);
	salvarIndice(projeto->nome_idx_wf,projeto->worst_fit);
}