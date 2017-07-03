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
#include <companhias.h>

/* Aloca uma nova estrutura companhia */
Companhia *criarCompanhia(int flag_alocar){
	Companhia *nova_companhia = (Companhia *) calloc(1,sizeof(Companhia));

	//aloca memoria para os campos de tamanho fixo
	if(flag_alocar == 1){
		nova_companhia->cnpj = (char *) malloc((TAMANHO_CNPJ + 1) * sizeof(char));
		nova_companhia->cnpj_auditoria = (char *) malloc((TAMANHO_CNPJ + 1) * sizeof(char));
		nova_companhia->data_registro = (char *) malloc((TAMANHO_DATA + 1) * sizeof(char));
		nova_companhia->data_cancelamento = (char *) malloc((TAMANHO_DATA + 1) * sizeof(char));
	}

	return nova_companhia;
}

/* Desaloca um estrutura companhia da memoria */
void destruirCompanhia(Companhia *companhia){
	if(companhia->cnpj) free(companhia->cnpj);
	if(companhia->nome_social) free(companhia->nome_social);
	if(companhia->nome_fantasia) free(companhia->nome_fantasia);
	if(companhia->data_registro) free(companhia->data_registro);
	if(companhia->data_cancelamento) free(companhia->data_cancelamento);
	if(companhia->motivo_cancelamento) free(companhia->motivo_cancelamento);
	if(companhia->nome_empresa)	free(companhia->nome_empresa);
	if(companhia->cnpj_auditoria) free(companhia->cnpj_auditoria);

	free(companhia);
}

/* Recebe uma estrutura que contem uma companhia e a imprime */
void imprimirCompanhia(Companhia *companhia){
	if(companhia->cnpj) printf("CNPJ: %s\n",companhia->cnpj);
	if(companhia->nome_social) printf("NOME SOCIAL: %s\n",companhia->nome_social);
	if(companhia->nome_fantasia) printf("NOME FANTASIA: %s\n",companhia->nome_fantasia);
	if(companhia->data_registro) printf("DATA REGISTRO: %s\n",companhia->data_registro);
	if(companhia->data_cancelamento) printf("DATA CANCELAMENTO: %s\n",companhia->data_cancelamento);
	if(companhia->motivo_cancelamento) printf("MOTIVO CANCELAMENTO: %s\n",companhia->motivo_cancelamento);
	if(companhia->nome_empresa) printf("NOME EMPRESA: %s\n",companhia->nome_empresa);
	if(companhia->cnpj_auditoria) printf("CNPJ AUDITORIA: %s\n",companhia->cnpj_auditoria);
}

/* Recebe um ponteiro para arquivo csv e lê um registro */
Companhia *lerCompanhiaCSV(FILE *input_file){
	Companhia *nova_companhia = criarCompanhia(1);

	
	fscanf(input_file,"%[^;];",nova_companhia->cnpj);
	if(!strcmp(nova_companhia->cnpj,"null")){
		free(nova_companhia->cnpj);
		nova_companhia->cnpj = NULL;
	}
	
	fscanf(input_file,"%m[^;];",&nova_companhia->nome_social);
	if(!strcmp(nova_companhia->nome_social,"null")){
		free(nova_companhia->nome_social);
		nova_companhia->nome_social = NULL;
	}
	
	fscanf(input_file,"%m[^;];",&nova_companhia->nome_fantasia);
	if(!strcmp(nova_companhia->nome_fantasia,"null")){
		free(nova_companhia->nome_fantasia);
		nova_companhia->nome_fantasia = NULL;
	}
	
	fscanf(input_file,"%[^;];",nova_companhia->data_registro);
	if(!strcmp(nova_companhia->data_registro,"null")){
		free(nova_companhia->data_registro);
		nova_companhia->data_registro = NULL;
	}
	
	fscanf(input_file,"%[^;];",nova_companhia->data_cancelamento);
	if(!strcmp(nova_companhia->data_cancelamento,"null")){
		free(nova_companhia->data_cancelamento);
		nova_companhia->data_cancelamento = NULL;
	}
	
	fscanf(input_file,"%m[^;];",&nova_companhia->motivo_cancelamento);
	if(!strcmp(nova_companhia->motivo_cancelamento,"null")){
		free(nova_companhia->motivo_cancelamento);
		nova_companhia->motivo_cancelamento = NULL;
	}
	
	fscanf(input_file,"%m[^;];",&nova_companhia->nome_empresa);
	if(!strcmp(nova_companhia->nome_empresa,"null")){
		free(nova_companhia->nome_empresa);
		nova_companhia->nome_empresa = NULL;
	}
	
	fscanf(input_file,"%[^\n]\n",nova_companhia->cnpj_auditoria);
	if(!strcmp(nova_companhia->cnpj_auditoria,"null")){
		free(nova_companhia->cnpj_auditoria);
		nova_companhia->cnpj_auditoria = NULL;
	}

	return nova_companhia;
}


/* Retorna o tamanho da companhia em bytes(leva em consideração os delimitadores que serão inseridos)*/
int getTamanhoCompanhia(Companhia *companhia){
	int tamanho = 0;
	
	tamanho += TAMANHO_CNPJ; //tamanho da string + '\0'
	tamanho += TAMANHO_CNPJ; //tamanho da string + '\0'
	tamanho += TAMANHO_DATA; //tamanho da string + '\0'
	tamanho += TAMANHO_DATA; //tamanho da string + '\0'

	if(companhia->nome_social) tamanho += strlen(companhia->nome_social)+1; //tamanho da string + '\0'
	tamanho++; //delimitador fim de campo

	if(companhia->nome_fantasia) tamanho += strlen(companhia->nome_fantasia)+1; //tamanho da string + '\0'
	tamanho++; //delimitador fim de campo

	if(companhia->motivo_cancelamento) tamanho += strlen(companhia->motivo_cancelamento)+1; //tamanho da string + '\0'
	tamanho++; //delimitador fim de campo

	if(companhia->nome_empresa) tamanho += strlen(companhia->nome_empresa)+1; //tamanho da string + '\0'
	tamanho++; //delimitador fim de campo

	tamanho++; //delimitador fim de registro

	return tamanho;
}

/* Le a entrada do usuario para criar nova companhia */
Companhia *lerCompanhiaIndividual(){
	Companhia *nova_companhia = criarCompanhia(1);

	printf("Entre com com a nova companhia(Digite 'null' nos campos que devem ser nulos):\n");

	
	printf("CNPJ (XX.XXX.XXX/XXXX-XX): ");
	scanf("%*c%[^\n]",nova_companhia->cnpj);
	if(!strcmp(nova_companhia->cnpj,"null")){
		free(nova_companhia->cnpj);
		nova_companhia->cnpj = NULL;
	}
	
	printf("Nome Social: ");
	scanf("%*c%m[^\n]",&nova_companhia->nome_social);
	if(!strcmp(nova_companhia->nome_social,"null")){
		free(nova_companhia->nome_social);
		nova_companhia->nome_social = NULL;
	}
	
	printf("Nome Fantasia: ");
	scanf("%*c%m[^\n]",&nova_companhia->nome_fantasia);
	if(!strcmp(nova_companhia->nome_fantasia,"null")){
		free(nova_companhia->nome_fantasia);
		nova_companhia->nome_fantasia = NULL;
	}
	
	printf("Data registro(DD/MM/AA): ");
	scanf("%*c%[^\n]",nova_companhia->data_registro);
	if(!strcmp(nova_companhia->data_registro,"null")){
		free(nova_companhia->data_registro);
		nova_companhia->data_registro = NULL;
	}
	
	printf("Data Cancelamento(DD/MM/AA): ");
	scanf("%*c%[^\n]",nova_companhia->data_cancelamento);
	if(!strcmp(nova_companhia->data_cancelamento,"null")){
		free(nova_companhia->data_cancelamento);
		nova_companhia->data_cancelamento = NULL;
	}
	
	printf("Motivo Cancelamento: ");
	scanf("%*c%m[^\n]",&nova_companhia->motivo_cancelamento);
	if(!strcmp(nova_companhia->motivo_cancelamento,"null")){
		free(nova_companhia->motivo_cancelamento);
		nova_companhia->motivo_cancelamento = NULL;
	}
	
	printf("Nome Auditoria: ");
	scanf("%*c%m[^\n]",&nova_companhia->nome_empresa);
	if(!strcmp(nova_companhia->nome_empresa,"null")){
		free(nova_companhia->nome_empresa);
		nova_companhia->nome_empresa = NULL;
	}
	
	printf("CNPJ Auditoria (XX.XXX.XXX/XXXX-XX): ");
	scanf("%*c%[^\n]",nova_companhia->cnpj_auditoria);
	if(!strcmp(nova_companhia->cnpj_auditoria,"null")){
		free(nova_companhia->cnpj_auditoria);
		nova_companhia->cnpj_auditoria = NULL;
	}

	return nova_companhia;
}