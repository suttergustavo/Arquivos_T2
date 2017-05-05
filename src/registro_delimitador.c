#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <registro_delimitador.h>

char *lerAteDelimitadorRegistro(FILE *in, char *delimitador){
	char c,*str = NULL;
	int count = 0;
	while(1){
		fread(&c,sizeof(char),1,in);
		if(c == EOF){
			free(str);
			str = NULL;
		}
		if(c == DELIM_FIM_CAMPO || c == DELIM_FIM_REG) break;

		str = (char *) realloc(str,sizeof(char)*(count + 1));
		str[count++] = c;
	}

	(*delimitador) = c;

	return str;
}

void escreverCompanhiaDelimitador(FILE *out,Companhia *companhia) {
	char delim_fim_campo = DELIM_FIM_CAMPO;
	char delim_fim_reg = DELIM_FIM_REG;

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

	fwrite(&delim_fim_reg, sizeof(char), 1, out); // escreve o delimitador de registro '#'
}

/* Le todos os registros de um arquivo */
Companhia** lerTodosDelimitador(FILE *in, int* n_regs){
	Companhia **companhias = NULL;
	int end, count = 0;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){
		companhias = realloc(companhias,sizeof(Companhia*)*(count+1));
		companhias[count++] = lerCompanhiaDelimitador(in);
	}


	*n_regs = count;
	return companhias;
}

/* Busca por registros atráves de um campo passado pelo usuário, e o retorna todos encontrados */
Companhia **buscarCampoDelimitador(FILE *in, Campo campo, char *query, int *n_regs){
	Companhia *companhia,**companhias = NULL;
	int end,count = 0;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){
		companhia = lerCompanhiaDelimitador(in);
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

/* Lê um registro de um arquivo verificando se o delimitador é um '|' ou um '#' */
Companhia *lerCompanhiaDelimitador(FILE *in){
	Companhia *companhia = criarCompanhia(0);
	char delimitador;
	
	companhia->cnpj = lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;
	companhia->nome_social = lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;
	companhia->nome_fantasia = lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;
	companhia->data_registro = lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;
	companhia->data_cancelamento = lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;
	companhia->motivo_cancelamento = lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;
	companhia->nome_empresa = lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;
	companhia->cnpj_auditoria = lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;

	lerAteDelimitadorRegistro(in, &delimitador);
	if (delimitador == DELIM_FIM_REG) return companhia;

	return NULL;
}

/* Busca por um registro atráves do nome fantasia, e o retorna */
Companhia *buscarNomeDelimitador(FILE *in, char *query){
	Companhia *companhia;
	int end,found = 0;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,0,SEEK_SET);

	while(ftell(in) < end){
		companhia = lerCompanhiaDelimitador(in);
		if(companhia->nome_fantasia && !strcmp(companhia->nome_fantasia,query)){
			found = 1;
			continue;
		}
		destruirCompanhia(companhia);
	}
	return found ? companhia : NULL;
}

/* Busca por um registro atráves do seu numero, e o retorna */
Companhia *buscarNumRegDelimitador(FILE *in, int query){
	Companhia *companhia = NULL;
	int end,found = 0;
	int count = 0;

	fseek(in,0,SEEK_END);
	end = ftell(in);
	fseek(in,0,SEEK_SET);


	while(ftell(in) < end){
		companhia = lerCompanhiaDelimitador(in);
		if(count++ == query){
			found = 1;
			continue;
		}
		destruirCompanhia(companhia);
	}
	return found ? companhia : NULL;
}