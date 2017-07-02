#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <companhias.h>
#include <registro_delimitador.h>
#include <indice.h>
#include <projeto.h>

int main(int argc, char *argv[]){
	char *nome_projeto;
	char *nome_csv;
	char *cnpj;
	Projeto *projeto;
	Companhia *companhia;

	printf("Digite o nome do projeto(Caso não exista ele será criado): ");
	scanf("%ms",&nome_projeto);

	projeto = iniciarProjeto(nome_projeto);

	int cmd = 1;
	while(cmd){
		printf("1- Importar de CSV | 2- Inserir Individual | 3- Remover Companhia | 4- Visualizar Indices | 5- Visualizar Lista Removidos | 0- Salvar e Sair\n");
		scanf("%d",&cmd);

		switch(cmd){
			case 1: //importar dados de CSV
				printf("Digite o nome da arquivo CSV\n");
				scanf("%ms",&nome_csv);
				inserirDoCSV(projeto,nome_csv);
				free(nome_csv);
				break;
			case 2: //inserir companhia individual
				companhia = lerCompanhiaIndividual();
				inserirCompanhiaIndividual(projeto,companhia);
				destruirCompanhia(companhia);
				break;
			case 3: //remover companhia
				printf("Digite o CNPJ que deve ser removido(XX.XXX.XXX/XXXX-XX): \n");
				scanf("%ms",&cnpj);
				removerCompanhia(projeto,cnpj);
				free(cnpj);
				break;
			case 4: //visualizar indices
				imprimirIndices(projeto);
				break;
			case 5: //visualizar lista removidos
				imprimirListas(projeto);
				break;
		}
	}

	//salvando indice em disco
	salvarIndices(projeto);
	//liberando memoria
	freeProjeto(projeto);
}	