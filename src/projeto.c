#include <stdlib.h>
#include <stdio.h.h>
#include <string.h>
#include <registro_delimitador.h>
#include <projeto.h>

/* Função usada simplesmente para concatenação para criar nomes de arquivos*/ 
char *criarNomeArquivo(char *str1,char *str2){
	char *res = (char *) malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));
	strcpy(res,str1);
	strcat(res,str2);
	return res;
}

/* Dado um nome um projeto é iniciado, ou recuperado do disco */
Projeto *iniciarProjeto(char *nome){
	Projeto *projeto = (Projeto) malloc(sizeof(Projeto));
	
	//nome que do projeto
	projeto->nome_projeto = nome;

	//nome dos arquivos que o projeto manipulará
	projeto->nome_dados_ff = criarNomeArquivo(nome,"_dadosFF.dat");
	projeto->nome_dados_bf = criarNomeArquivo(nome,"_dadosBF.dat");
	projeto->nome_dados_wf = criarNomeArquivo(nome,"_dadosWF.dat");
	projeto->nome_idx_bf = criarNomeArquivo(nome,"_indiceFF.dat");
	projeto->nome_idx_ff = criarNomeArquivo(nome,"_indiceBF.dat");
	projeto->nome_idx_wf = criarNomeArquivo(nome,"_indiceWF.dat");
	
	//carrega(ou cria caso,não existam) os indices do projeto
	projeto->first_fit = carregarIndice(projeto->nome_idx_ff);
	projeto->best_fit = carregarIndice(projeto->nome_idx_bf);
	projeto->worst_fit = carregarIndice(projeto->nome_idx_wf);

	return projeto;
}

void inserir(){

}