#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <companhias.h>


/* Aloca uma nova estrutura companhia */
Companhia *criarCompanhia(int flag_alocar){
	Companhia *nova_companhia = (Companhia *) calloc(1,sizeof(Companhia));

	//aloca memoria para os campos de tamanho fixo
	if(flag_alocar == 1){
		nova_companhia->cnpj = (char *) malloc(TAMANHO_CNPJ * sizeof(char));
		nova_companhia->cnpj_auditoria = (char *) malloc(TAMANHO_CNPJ * sizeof(char));
		nova_companhia->data_registro = (char *) malloc(TAMANHO_DATA * sizeof(char));
		nova_companhia->data_cancelamento = (char *) malloc(TAMANHO_DATA * sizeof(char));
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

/* Retorna um determinado campo de uma companhia */
char *getCampoCompanhia(Companhia *companhia, Campo campo){
	if(campo == CNPJ) return companhia->cnpj;
	if(campo == NOME_SOCIAL) return companhia->nome_social;
	if(campo == NOME_FANTASIA) return companhia->nome_fantasia;
	if(campo == DATA_REGISTRO) return companhia->data_registro;
	if(campo == DATA_CANCELAMENTO) return companhia->data_cancelamento;
	if(campo == MOTIVO_CANCELAMENTO) return companhia->motivo_cancelamento;
	if(campo == NOME_EMPRESA) return companhia->nome_empresa;
	if(campo == CNPJ_AUDITORIA) return companhia->cnpj_auditoria;
	return NULL;
}

/* Checa se o uma compania tem um campo igual a uma query */
int possuiCampoProcurado(Companhia *companhia, Campo campo, char *query){
	if(campo == CNPJ){
		if(companhia->cnpj && !strcmp(companhia->cnpj,query)) return 1;
		return 0;
	}
	else if(campo == NOME_SOCIAL){
		if(companhia->nome_social && !strcmp(companhia->nome_social,query)) return 1;
		return 0;
	}
	else if(campo == NOME_FANTASIA){
		if(companhia->nome_fantasia && !strcmp(companhia->nome_fantasia,query)) return 1;
		return 0;	
	}
	else if(campo == DATA_REGISTRO){
		if(companhia->data_registro && !strcmp(companhia->data_registro,query)) return 1;
		return 0;
	}
	else if(campo == DATA_CANCELAMENTO){
		if(companhia->data_cancelamento && !strcmp(companhia->data_cancelamento,query)) return 1;
		return 0;
	}
	else if(campo == MOTIVO_CANCELAMENTO){
		if(companhia->motivo_cancelamento && !strcmp(companhia->motivo_cancelamento,query)) return 1;
		return 0;
	}
	else if(campo == NOME_EMPRESA){
		if(companhia->nome_empresa && !strcmp(companhia->nome_empresa,query)) return 1;
		return 0;
	}
	else if(campo == CNPJ_AUDITORIA){
		if(companhia->cnpj_auditoria && !strcmp(companhia->cnpj_auditoria,query)) return 1;
		return 0;
	}
	return 0;
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


/* Recebe um nome de arquivo CSV e retorna todos os registros contidos nele */
Companhia **lerCSVCompleto(char *filename, int *n_companhias){
	FILE *in;
	Companhia **companhias = NULL;
	int end,count = 0;
	*n_companhias = 0;

	in = fopen(filename,"r");
	if(in == NULL) return NULL;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){
		companhias = (Companhia**) realloc(companhias,sizeof(Companhia*)*(count+1));
		companhias[count++] = lerCompanhiaCSV(in);
	}

	fclose(in);

	*n_companhias = count;
	return companhias;
}
