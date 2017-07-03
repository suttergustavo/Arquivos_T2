/*
Autores:

Gustavo Sutter - 9763193
Matheus Gomes - 9779270
Guilherme Montemovo - 9779461
Joice Aurino - 8530851

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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

	struct stat st = {0};
	if (stat(nome, &st) == -1) mkdir(nome, 0700);

	//nome dos arquivos que o projeto manipulará
	projeto->nome_dados_ff = criarNomeArquivo(nome,"/dadosFF.dat");
	projeto->nome_dados_bf = criarNomeArquivo(nome,"/dadosBF.dat");
	projeto->nome_dados_wf = criarNomeArquivo(nome,"/dadosWF.dat");
	projeto->nome_idx_ff = criarNomeArquivo(nome,"/indiceFF.dat");
	projeto->nome_idx_bf = criarNomeArquivo(nome,"/indiceBF.dat");
	projeto->nome_idx_wf = criarNomeArquivo(nome,"/indiceWF.dat");
	
	//carrega(ou cria caso,não existam) os indices do projeto
	projeto->first_fit = carregarIndice(projeto->nome_idx_ff, projeto->nome_dados_ff);
	projeto->best_fit = carregarIndice(projeto->nome_idx_bf, projeto->nome_dados_bf);
	projeto->worst_fit = carregarIndice(projeto->nome_idx_wf, projeto->nome_dados_wf);

	projeto->alterado = 0;

	return projeto;
}

/* Insere as companhias de um arquivo csv nos arquivos dados e nos indices */
void inserirDoCSV(Projeto *projeto, char *arquivo_csv){
	FILE *csv = fopen(arquivo_csv,"r");
	Companhia *companhia;
	int offset;
 
 	//caso o csv não exista
	if(csv == NULL){
		printf("Arquivo CSV não encontrado\n");
		return;
	}
 	
	fseek(csv,0,SEEK_END);
	int size = (int) ftell(csv);	
	fseek(csv,0,SEEK_SET);
	
	//muda a validade do arquivo de indice, caso nao isso não tenha sido feito
	if(projeto->alterado == 0){
		projeto->alterado = 1;
		validadeIndice(projeto->nome_idx_ff,INVALIDO);
		validadeIndice(projeto->nome_idx_bf,INVALIDO);
		validadeIndice(projeto->nome_idx_wf,INVALIDO);
	}
 
 	//enquanto não está no fim do arquivo
	while(size > (int) ftell(csv)){
		companhia = lerCompanhiaCSV(csv);

		offset = escreverCompanhia(projeto->nome_dados_ff,companhia,FIRST_FIT);
		inserirIndice(projeto->first_fit,companhia->cnpj,offset);

		offset = escreverCompanhia(projeto->nome_dados_bf,companhia,BEST_FIT);
		inserirIndice(projeto->best_fit,companhia->cnpj,offset);

		offset = escreverCompanhia(projeto->nome_dados_wf,companhia,WORST_FIT);
		inserirIndice(projeto->worst_fit,companhia->cnpj,offset);
	
		destruirCompanhia(companhia);
	}

	fclose(csv);
}

/* Remove uma companhia dos arquivos de dados e dos indices*/
void removerCompanhia(Projeto *projeto, char *cnpj){
	int offset;

	if(projeto->alterado == 0){
		projeto->alterado = 1;
		validadeIndice(projeto->nome_idx_ff,INVALIDO);
		validadeIndice(projeto->nome_idx_bf,INVALIDO);
		validadeIndice(projeto->nome_idx_wf,INVALIDO);
	}

	offset = removerIndice(projeto->first_fit,cnpj);
	if(offset != -1) removerRegistro(projeto->nome_dados_ff,offset,FIRST_FIT);
	else{//caso não tenha sido encontrado
		printf("Não foi possivel remover. CNPJ não encontrado\n");
		return; //pode retornar porque se não está em um indice não está em nenhum
	}

	offset = removerIndice(projeto->best_fit,cnpj);
	if(offset != -1) removerRegistro(projeto->nome_dados_bf,offset,BEST_FIT);

	offset = removerIndice(projeto->worst_fit,cnpj);
	if(offset != -1) removerRegistro(projeto->nome_dados_wf,offset,WORST_FIT);
}

/* Insere uma companhia nos arquivos de dados e nos indices */
void inserirCompanhiaIndividual(Projeto *projeto, Companhia *companhia){
	int offset;

	if(projeto->alterado == 0){
		projeto->alterado = 1;
		validadeIndice(projeto->nome_idx_ff,INVALIDO);
		validadeIndice(projeto->nome_idx_bf,INVALIDO);
		validadeIndice(projeto->nome_idx_wf,INVALIDO);
	}

	offset = escreverCompanhia(projeto->nome_dados_ff,companhia,FIRST_FIT);
	inserirIndice(projeto->first_fit,companhia->cnpj,offset);

	offset = escreverCompanhia(projeto->nome_dados_bf,companhia,BEST_FIT);
	inserirIndice(projeto->best_fit,companhia->cnpj,offset);

	offset = escreverCompanhia(projeto->nome_dados_wf,companhia,WORST_FIT);
	inserirIndice(projeto->worst_fit,companhia->cnpj,offset);
}

/* Imprime em formato de tabela os indices */
void imprimirIndices(Projeto *projeto){
	printf("---------------------------------------\n");
	printf("|%19s|%6s|%6s|%6s|\n","CNPJ      ","First","Best","Worst");
	printf("---------------------------------------\n");
	for(int i=0;i<projeto->first_fit->size;i++)
		printf("|%19s|%6d|%6d|%6d|\n",projeto->first_fit->indice[i]->cnpj,projeto->first_fit->indice[i]->offset,projeto->best_fit->indice[i]->offset,projeto->worst_fit->indice[i]->offset);
	printf("---------------------------------------\n");
	printf("\n");
}

/* Imprime as listas de espaços para reuso */
void imprimirListas(Projeto *projeto){
	imprimirListaRemovidos(projeto->nome_dados_ff,"First Fit");
	imprimirListaRemovidos(projeto->nome_dados_bf,"Best Fit");
	imprimirListaRemovidos(projeto->nome_dados_wf,"Worst Fit");
}

/* Salva os indices no disco */
void salvarIndices(Projeto *projeto){
	salvarIndice(projeto->nome_idx_ff,projeto->first_fit);
	salvarIndice(projeto->nome_idx_bf,projeto->best_fit);
	salvarIndice(projeto->nome_idx_wf,projeto->worst_fit);
}

/* Libera as estruturas do projeto da memoria*/
void freeProjeto(Projeto *projeto){
	free(projeto->nome_projeto);
	free(projeto->nome_dados_ff);
	free(projeto->nome_dados_bf);
	free(projeto->nome_dados_wf);
	free(projeto->nome_idx_bf);
	free(projeto->nome_idx_ff);
	free(projeto->nome_idx_wf);

	destruirIndice(projeto->best_fit);
	destruirIndice(projeto->worst_fit);
	destruirIndice(projeto->first_fit);

	free(projeto);
}