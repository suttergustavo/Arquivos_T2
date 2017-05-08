#include <stdlib.h>
#include <stdio.h>
#include <companhias.h>
#include <manipulacao_arquivos.h>

int main(){
    int op, estrutura, n_regs, i, j, nr, n_campo, loop = 1;
    char *string_busca;
    char *string_campo;
    char string_nome_arquivo[20];
    Companhia **cs,*c;

    printf("\n Escolha uma estrutura: ");
    printf("\n\n 1-Registro com indicador de tamanho");
    printf("\n 2-Registro com delimitador");
    printf("\n 3-Registro com numero fixo de campos");
    printf("\n\n >>Opcao:  ");
    scanf("%d", &estrutura);

    while(loop == 1){
    	printf("\n\n Escolha uma das opcoes abaixo: ");
        printf("\n\n 1- Fornecer arquivo de entrada para leitura de registros ");
        printf("\n 2- Recuperar todos os dados dos registros");
        printf("\n 3- Buscar por Campo ");
        printf("\n 4- Buscar por Numero de Registro: ");
        printf("\n 5- Busca por Campo espicifico de um Registro: ");
        printf("\n 0- Sair");
        printf("\n\n >>Opcao:  ");
        scanf("%d", &op);

        switch(op){

        	case 1: //LER REGISTROS DE UM ARQUIVO CSV
        		printf ("\nDigite o nome do arquivo: ");
        		scanf ("%s", string_nome_arquivo);
				cs = lerCSVCompleto(string_nome_arquivo,&n_regs);
        		for(i = 0; i < n_regs; i++)
        			escreverCompanhia("out",cs[i],estrutura);
        		for(j = 0;j < n_regs; j++)
        	    	destruirCompanhia(cs[j]);
        	    free(cs);

				printf("\n\n Deseja realizar mais operacoes?");
				printf("\n\n 1- Sim");
				printf("\n 2- Nao");
				printf("\n\n >>Opcao: ");
				scanf("%d", &loop);

            	break;

			case 2: //LER TODAS AS COMPANHIAS DE UM ARQUIVO DE DADOS
				cs = lerTodasCompanhias("out",&n_regs, estrutura);
				for(i=0; i<n_regs; ++i){
					printf("\n\n");
					imprimirCompanhia(cs[i]);
					printf("------------\n");
					destruirCompanhia(cs[i]);
				}
				free(cs);
			
				printf("\n\n Deseja realizar mais peracoes?");
				printf("\n\n 1- Sim");
				printf("\n 2- Nao");
				printf("\n\n >>Opcao: ");
				scanf("%d", &loop);
			
				break;

			case 3: //FAZER BUSCA POR CAMPO
				printf("\n\n Escolha o Campo: ");
				printf("\n 1- CNPJ");
				printf("\n 2- Nome Social");
				printf("\n 3- Nome fantasia");
				printf("\n 4- Data de registro");
				printf("\n 5- Data de cancelamento");
				printf("\n 6- Nome da empresa");
				printf("\n 7- CNPJ da empresa de auditoria");
				printf("\n\n >>Opcao: ");
				scanf("%d", &n_campo);
			
				printf("\n\n Digite a chave de busca: ");
				scanf("%ms", &string_busca);
				cs = buscarPorCampo("out",n_campo, string_busca,&n_regs,estrutura);
				for(i=0; i<n_regs; ++i){
					imprimirCompanhia(cs[i]);
					printf("------------\n");
					destruirCompanhia(cs[i]);
				}
				free(string_busca);
				free(cs);

				printf("\n\n Deseja realizar mais peracoes?");
				printf("\n\n 1- Sim");
				printf("\n 2- Nao");
				printf("\n\n >>Opcao: ");
				scanf("%d", &loop);

        	    break;

			case 4: //FAZER BUSCA POR POSIÇÃO
				printf("\n\n Numero do Registro: ");
				scanf("%d", &nr);
			
				c = buscarPorPosicao("out",nr,estrutura);
				if(c)imprimirCompanhia(c);
				else printf("\n\n ***Nao foi possivel recuperar esse registro***");
				printf("------------\n");
				if(c)destruirCompanhia(c);
			
				printf("\n\n Deseja realizar mais peracoes?");
				printf("\n 1- Sim");
				printf("\n 2- Nao");
				printf("\n\n >>Opcao: ");
				scanf("%d", &loop);
			
				break;


			case 5: //RETORNAR CAMPO DE REGISTRO POR POSI
				printf("\n\n Escolha o Campo: ");
				printf("\n 1- CNPJ");
				printf("\n 2- Nome Social");
				printf("\n 3- Nome fantasia");
				printf("\n 4- Data de registro");
				printf("\n 5- Data de cancelamento");
				printf("\n 6- Nome da empresa");
				printf("\n 7- CNPJ da empresa de auditoria");
				printf("\n\n >>Opcao: ");
				scanf("%d", &n_campo);
				printf("\n\n Numero do Registro: ");
				scanf("%d", &nr);
				c = buscarPorPosicao("out",nr,estrutura);

				if(n_campo == 1){
					string_campo = getCampoCompanhia(c, CNPJ);
					printf("\n\n CNPJ: %s", string_campo);
				}

				if(n_campo == 2){
					string_campo = getCampoCompanhia(c, NOME_SOCIAL);
					printf("\n\n Nome Social: %s", string_campo);
				}

				if(n_campo == 3){
					string_campo = getCampoCompanhia(c, NOME_FANTASIA);
					printf("\n\n Nome Fantasia: %s", string_campo);
				}

				if(n_campo == 4){
					string_campo = getCampoCompanhia(c, DATA_REGISTRO);
					printf("\n\n Data de Registro: %s", string_campo);
				}

				if(n_campo == 5){
					string_campo = getCampoCompanhia(c, DATA_CANCELAMENTO);
					printf("\n\n Data de Cancelamento: %s", string_campo);
				}

				if(n_campo == 6){
					string_campo = getCampoCompanhia(c, NOME_EMPRESA);
					printf("\n\n Nome da Empresa: %s", string_campo);
				}

				if(n_campo == 7){
					string_campo = getCampoCompanhia(c, CNPJ_AUDITORIA);
					printf("\n\n CNPJ da empresa de auditoria: %s", string_campo);
				}

				printf("\n\n Deseja realizar mais peracoes?");
				printf("\n\n 1- Sim");
				printf("\n 2- Nao");
				printf("\n\n >>Opcao: ");
				scanf("%d", &loop);

				break;

			case 0:
				exit(op);

			default:
				printf("\n\n A opcao digitada esta incorreta, deseja tentar novamente?");
				printf("\n 1- Sim");
				printf("\n 0- Nao");
				printf("\n\n >>Opcao: ");
				scanf("%d", &loop);
		}
	}

	return 0;
}	
