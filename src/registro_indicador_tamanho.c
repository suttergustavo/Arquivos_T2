/*
Autores:

Gustavo Sutter - 9763193
Matheus Gomes - 9779270
Guilherme Montemovo - 9779461
Joice Aurino - 8530851

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <registro_indicador_tamanho.h>

/* Le o campo do arquivo até achar um delimitador */
char *lerAteDelimitadorTamReg(FILE *in,int*tam){
	char c,*str = NULL;
	int count = 0;
	while(*tam > 0){ //le enquanto não achar um fim de campo ou o registro não acabar
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
	return str;
}

/* Le o tamanho do proximo registro no arquivo  */
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

	while(ftell(in) < end){ //enquanto o arquivo não chegar no fim
		//le e armazena os registros
		companhias = realloc(companhias,sizeof(Companhia*)*(count+1));
		companhias[count++] = lerCompanhiaTamReg(in);
	}


	*n_regs = count; //numero de registros que serão retornados
	return companhias;
}

/* Lê um registro de um arquivo */
Companhia *lerCompanhiaTamReg(FILE *in){
	Companhia *companhia = criarCompanhia(0);
	int tamanho_registro;

	tamanho_registro = getTamanhoTamReg(in); //pega tamanho do registro

	//le cada campo do registro
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
		companhia = lerCompanhiaTamReg(in); //le o proximo registro
		if(possuiCampoProcurado(companhia,campo,query)){ //caso seja o procura
			//armazena no vetor que será retornado
			companhias = (Companhia**) realloc(companhias,sizeof(Companhia*)*(count+1));
			companhias[count++] = companhia;
		}else{
			//caso contrario não é preciso armazena-lo
			destruirCompanhia(companhia);
		}
	}

	*n_regs = count; //numero de registros que sera retornado
	return companhias;
}

/* Busca por um registro atráves do seu numero, e o retorna */
Companhia *buscarNumRegTamReg(FILE *in, int query){
	Companhia *companhia = NULL;
	int tam,end;
	int count = 0;

	fseek(in,0,SEEK_END);
	end = ftell(in);
	fseek(in,0,SEEK_SET);


	while(ftell(in) < end){
		if(++count == query){ //caso esteja no registro de procurado
			companhia = lerCompanhiaTamReg(in); //armazena o registro
			break;
		}else{ //caso contrario
			tam = getTamanhoTamReg(in); //le o tamanho registro atual
			fseek(in,tam,SEEK_CUR); //vai para o proximo registro
		}
	}
	return companhia;
}

/* Retorna o tamanho do registro*/
int getTamanhoCompanhia(Companhia *companhia){
	int tam = 0;

	//soma cada campo(caso exista) e os delimitadores entre campos
	if(companhia->cnpj)tam += strlen(companhia->cnpj) + 1;
	tam++;
	if(companhia->nome_social)tam += strlen(companhia->nome_social) + 1;
	tam++;
	if(companhia->nome_fantasia)tam += strlen(companhia->nome_fantasia) + 1;
	tam++;
	if(companhia->data_registro)tam += strlen(companhia->data_registro) + 1;
	tam++;
	if(companhia->data_cancelamento)tam += strlen(companhia->data_cancelamento) + 1;
	tam++;
	if(companhia->motivo_cancelamento)tam += strlen(companhia->motivo_cancelamento) + 1;
	tam++;
	if(companhia->nome_empresa)tam += strlen(companhia->nome_empresa) + 1;
	tam++;
	if(companhia->cnpj_auditoria)tam += strlen(companhia->cnpj_auditoria) + 1;
	tam++;

	return tam;
}

/* Escreve uma companhia em um arquivo */
void escreverCompanhiaTamReg(FILE *out, Companhia *companhia){
	char delim_fim_campo = DELIM_FIM_CAMPO;

	int tam = getTamanhoCompanhia(companhia); //recupera o tamanho do registro

	fwrite(&tam,sizeof(int),1,out); //escreve o tamanho do registro

	//escreve cada campo utilizando o delimidor entre eles
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