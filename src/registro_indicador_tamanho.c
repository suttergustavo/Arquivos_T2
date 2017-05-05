#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <registro_indicador_tamanho.h>

/* Le o campo do arquivo até achar um delimitador */
char *lerAteDelimitadorTamReg(FILE *in,int*tam){
	char c,*str = NULL;
	int count = 0;
	while(*tam > 0){
		fread(&c,sizeof(char),1,in);
		(*tam)--;
		if(c == EOF){
			free(str);
			str = NULL;
		}
		if(c == DELIM_FIM_CAMPO) break;
		str = (char *) realloc(str,sizeof(char)*(count + 1));
		str[count++] = c;
	}

	printf("%d\n",*tam);
	return str;
}

int getTamanhoTamReg(FILE *in){
	int tamanho_registro;
	fread(&tamanho_registro,sizeof(int),1,in);
	return tamanho_registro;
}

/* Le todos os registros de um arquivo */
Companhia** lerTodosTamReg(FILE *in, int *n_regs){
	Companhia **companhias = NULL;
	int end, count = 0;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){
		printf("%d < %d\n",(int) ftell(in),end);
		companhias = realloc(companhias,sizeof(Companhia*)*(count+1));
		companhias[count++] = lerCompanhiaTamReg(in);
	}


	*n_regs = count;
	return companhias;
}

/* Lê um registro de um arquivo */
Companhia *lerCompanhiaTamReg(FILE *in){
	Companhia *companhia = criarCompanhia(0);
	int tamanho_registro;

	tamanho_registro = getTamanhoTamReg(in);

	if(tamanho_registro < 7) return NULL;

	companhia->cnpj = lerAteDelimitadorTamReg(in,&tamanho_registro);
	companhia->nome_social = lerAteDelimitadorTamReg(in,&tamanho_registro);
	companhia->nome_fantasia = lerAteDelimitadorTamReg(in,&tamanho_registro);
	companhia->data_registro = lerAteDelimitadorTamReg(in,&tamanho_registro);
	companhia->data_cancelamento = lerAteDelimitadorTamReg(in,&tamanho_registro);
	companhia->motivo_cancelamento = lerAteDelimitadorTamReg(in,&tamanho_registro);
	companhia->nome_empresa = lerAteDelimitadorTamReg(in,&tamanho_registro);
	companhia->cnpj_auditoria = lerAteDelimitadorTamReg(in,&tamanho_registro);
	
	return companhia;
}

/* Busca por registros atráves de um campo passado pelo usuário, e o retorna todos encontrados */
Companhia **buscarCampoTamReg(FILE *in, Campo campo, char *query, int *n_regs){
	Companhia *companhia,**companhias = NULL;
	int end,count = 0;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){
		companhia = lerCompanhiaTamReg(in);
		if(possuiCampoProcurado(companhia,campo,query)){
			companhias = (Companhia**) realloc(companhias,sizeof(Companhia*)*(count+1));
			companhias[count++] = companhia;
		}else{
			destruirCompanhia(companhia);
		}
	}

	*n_regs = count;
	return companhias;
}

/* Busca por um registro atráves do seu numero, e o retorna */
Companhia *buscarNumRegTamReg(FILE *in, int query){
	Companhia *companhia = NULL;
	int tam,end,found = 0;
	int count = 0;

	fseek(in,0,SEEK_END);
	end = ftell(in);
	fseek(in,0,SEEK_SET);


	while(ftell(in) < end){
		if(++count == query){
			companhia = lerCompanhiaTamReg(in);
			found = 1;
			continue;
		}else{
			tam = getTamanhoTamReg(in);
			fseek(in,tam,SEEK_SET);
		}
		destruirCompanhia(companhia);
	}
	return found ? companhia : NULL;
}

int getTamanhoCompanhia(Companhia *companhia){
	int tam = 0;

	if(companhia->cnpj)tam += strlen(companhia->cnpj) + 2;
	if(companhia->nome_social)tam += strlen(companhia->nome_social) + 2;
	if(companhia->nome_fantasia)tam += strlen(companhia->nome_fantasia) + 2;
	if(companhia->data_registro)tam += strlen(companhia->data_registro) + 2;
	if(companhia->data_cancelamento)tam += strlen(companhia->data_cancelamento) + 2;
	if(companhia->motivo_cancelamento)tam += strlen(companhia->motivo_cancelamento) + 2;
	if(companhia->nome_empresa)tam += strlen(companhia->nome_empresa) + 2;
	if(companhia->cnpj_auditoria)tam += strlen(companhia->cnpj_auditoria) + 2;

	return tam;
}

/* Escreve uma companhia em um arquivo */
void escreverCompanhiaTamReg(FILE *out, Companhia *companhia){
	char delim_fim_campo = DELIM_FIM_CAMPO;

	int tam = getTamanhoCompanhia(companhia);

	fwrite(&tam,sizeof(int),1,out);

	if(companhia->cnpj) fwrite(companhia->cnpj,sizeof(char),strlen(companhia->cnpj)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->nome_social) fwrite(companhia->nome_social,sizeof(char),strlen(companhia->nome_social)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->nome_fantasia) fwrite(companhia->nome_fantasia,sizeof(char),strlen(companhia->nome_fantasia)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->data_registro) fwrite(companhia->data_registro,sizeof(char),strlen(companhia->data_registro)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->data_cancelamento) fwrite(companhia->data_cancelamento,sizeof(char),strlen(companhia->data_cancelamento)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->motivo_cancelamento) fwrite(companhia->motivo_cancelamento,sizeof(char),strlen(companhia->motivo_cancelamento)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->nome_empresa) fwrite(companhia->nome_empresa,sizeof(char),strlen(companhia->nome_empresa)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->cnpj_auditoria) fwrite(companhia->cnpj_auditoria,sizeof(char),strlen(companhia->cnpj_auditoria)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);

}