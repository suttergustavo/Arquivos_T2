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
#include <registro_delimitador.h>

/* Escreve companhia no arquivo de dados e retorna o byte offset */
int escreverCompanhia(FILE *out,Companhia *companhia) {
	char delim_fim_campo = DELIM_FIM_CAMPO;
	char delim_fim_reg = DELIM_FIM_REG;

	int offset = (int) ftell(out);

	//escree cada campo e um delimitador entre eles
	if(companhia->cnpj) fwrite(companhia->cnpj,sizeof(char),strlen(companhia->cnpj)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->cnpj_auditoria) fwrite(companhia->cnpj_auditoria,sizeof(char),strlen(companhia->cnpj_auditoria)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->data_registro) fwrite(companhia->data_registro,sizeof(char),strlen(companhia->data_registro)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->data_cancelamento) fwrite(companhia->data_cancelamento,sizeof(char),strlen(companhia->data_cancelamento)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);

	if(companhia->nome_social) fwrite(companhia->nome_social,sizeof(char),strlen(companhia->nome_social)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->nome_fantasia) fwrite(companhia->nome_fantasia,sizeof(char),strlen(companhia->nome_fantasia)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->motivo_cancelamento) fwrite(companhia->motivo_cancelamento,sizeof(char),strlen(companhia->motivo_cancelamento)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	
	if(companhia->nome_empresa) fwrite(companhia->nome_empresa,sizeof(char),strlen(companhia->nome_empresa)+1,out);
	fwrite(&delim_fim_campo,sizeof(char),1,out);
	

	//escreve delimitador de fim de registro
	fwrite(&delim_fim_reg, sizeof(char), 1, out); // escreve o delimitador de registro '#'

	return offset;
}

/* Imprime todos os registros de um arquivo */
void imprimirTodos(FILE *in){
	Companhia *companhia;
	int end;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);
 
	while(ftell(in) < end){//enquanto não chegar no fim do arquivo
		companhia = lerCompanhia(in);
		if(companhia) {
			imprimirCompanhia(companhia);
			printf("----------------\n");
			destruirCompanhia(companhia);
		}
	}
}

/* Busca por registros atráves de um campo passado pelo usuário, e o retorna todos encontrados */
Companhia **buscarCampoDelimitador(FILE *in, Campo campo, char *query, int *n_regs){
	Companhia *companhia,**companhias = NULL;
	int end,count = 0;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){
		companhia = lerCompanhia(in);
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

void salvarCampoCompanhia(Companhia* companhia, Campo campo, char *valor){
	if(campo == CNPJ) companhia->cnpj = valor;
	if(campo == CNPJ_AUDITORIA) companhia->cnpj_auditoria = valor;
	if(campo == DATA_REGISTRO) companhia->data_registro = valor;
	if(campo == DATA_CANCELAMENTO) companhia->data_cancelamento = valor;
	if(campo == NOME_SOCIAL) companhia->nome_social = valor;
	if(campo == NOME_FANTASIA) companhia->nome_fantasia = valor;
	if(campo == MOTIVO_CANCELAMENTO) companhia->motivo_cancelamento = valor;
	if(campo == NOME_EMPRESA) companhia->nome_empresa = valor;
}

/* Lê um registro de um arquivo verificando se o delimitador é um '|' ou um '#' */
Companhia *lerCompanhia(FILE *in){
	Companhia *companhia = criarCompanhia(0);
	char *valor_campo = NULL;
	char c;
	int campo = 1;
	int count = 0;
	int run = 1;

	while(run){
		fread(&c,sizeof(char),1,in);
		if(c == FLAG_REMOVIDO){
			companhia = NULL;
			run = 0;
			do{
				fread(&c,sizeof(char),1,in);
				// printf("%c ",c);
			}while(c != DELIM_FIM_REG);
		}else if(c == DELIM_FIM_CAMPO){
			// printf(">> %s\n",valor_campo);
			salvarCampoCompanhia(companhia,campo,valor_campo);
			campo++;
			valor_campo = NULL;
			count = 0;
		}else if(c == DELIM_FIM_REG){
			run = 0; //chegou ao fim do registro
		}else{
			valor_campo = (char*) realloc(valor_campo,sizeof(char)*(count+1));
			valor_campo[count++] = c;
		}
	}

	return companhia;
}


/* Busca por um registro atráves do seu numero, e o retorna */
Companhia *buscarNumRegDelimitador(FILE *in, int query){
	Companhia *companhia = NULL;
	int end;
	int count = 0;

	fseek(in,0,SEEK_END);
	end = ftell(in);
	fseek(in,0,SEEK_SET);


	while(ftell(in) < end){ //enquanto não chegou no fim do arquivo
		companhia = lerCompanhia(in); //le a companhia
		if(++count == query) //caso seja a procurada
			break;
		destruirCompanhia(companhia); //caso não seja não é preciso armazena-la
		companhia = NULL;
	}

	return companhia;
}