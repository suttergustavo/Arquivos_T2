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
#include <registro_num_fixo_campos.h>

/* Le o campo do arquivo até achar um delimitador */
char *lerAteDelimitador(FILE *in){
	char c,*str = NULL;
	int count = 0;
	while(1){ //le e armazena chars até encontrar um delimitador
		fread(&c,sizeof(char),1,in);
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

/* Le todos os registros de um arquivo */
Companhia** lerTodosNumFixo(FILE *in, int* n_regs){
	Companhia **companhias = NULL;
	int end, count = 0;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){//enquanto ainda não chegou no fim do arquivo
		//guarda o registro lido em um vetor
		companhias = realloc(companhias,sizeof(Companhia*)*(count+1));
		companhias[count++] = lerCompanhiaNumFixo(in); 
	}

	*n_regs = count; //numero de registros que serão retornados
	return companhias;
}

/* Lê um registro de um arquivo */
Companhia *lerCompanhiaNumFixo(FILE *in){
	Companhia *companhia = criarCompanhia(0);
	
	//para cada um dos campos realiza a leitura até encontrar um delimitador
	companhia->cnpj = lerAteDelimitador(in);
	companhia->nome_social = lerAteDelimitador(in);
	companhia->nome_fantasia = lerAteDelimitador(in);
	companhia->data_registro = lerAteDelimitador(in);
	companhia->data_cancelamento = lerAteDelimitador(in);
	companhia->motivo_cancelamento = lerAteDelimitador(in);
	companhia->nome_empresa = lerAteDelimitador(in);
	companhia->cnpj_auditoria = lerAteDelimitador(in);

	return companhia;
}

/* Busca por registros atráves de um campo passado pelo usuário, e o retorna todos encontrados */
Companhia **buscarCampoNumFixo(FILE *in, Campo campo, char *query, int *n_regs){
	Companhia *companhia,**companhias = NULL;
	int end,count = 0;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){
		companhia = lerCompanhiaNumFixo(in); //le uma companhia
		if(possuiCampoProcurado(companhia,campo,query)){ //checa se a companhia é a buscada
			companhias = (Companhia**) realloc(companhias,sizeof(Companhia*)*(count+1)); //caso seja guarda no vetor que sera retornado
			companhias[count++] = companhia;
		}else{
			destruirCompanhia(companhia); //caso contrario não é preciso armazena-la
		}
	}

	*n_regs = count; //"retorna" via passagem por referencia quantos registros serão retornados
	return companhias;
}

/* Busca por um registro atráves do seu numero, e o retorna */
Companhia *buscarNumRegNumFixo(FILE *in, int query){
	Companhia *companhia = NULL;
	int end;	int count = 0;

	//pega o tamanho do arquivo onde sera realizada a busca
	fseek(in,0,SEEK_END);
	end = ftell(in);
	fseek(in,0,SEEK_SET);

	//vai de registro em registro procurando
	while(ftell(in) < end){
		companhia = lerCompanhiaNumFixo(in);
		if(++count == query)
			break;
		destruirCompanhia(companhia);
		companhia = NULL;
	}
	return companhia;
}

/* Escreve uma companhia em um arquivo */
void escreverCompanhiaNumFixo(FILE *out, Companhia *companhia){
	char delim_fim_campo = DELIM_FIM_CAMPO;

	//escreve os campos seguido de um delimitador
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