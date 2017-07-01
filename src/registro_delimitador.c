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
#include <indice.h>

/* Escreve companhia no arquivo de dados e retorna o byte offset */
int escreverCompanhia(char *filename,Companhia *companhia) {
	char delim_fim_campo = DELIM_FIM_CAMPO;
	char delim_fim_reg = DELIM_FIM_REG;

	FILE *out = NULL;
	out = fopen(filename,"r+");
	if(out == NULL)	out = fopen(filename,"w+"); //caso o arquivo não exista ele é criado 

	//verificando se o arquivo possui cabeçalho(ou seja, se n acabou de ser criado)
	fseek(out,0,SEEK_END);
	if(ftell(out) == 0){//caso não tenha nada no arquivo
		int cabeca_lista = -1;
		fwrite(&cabeca_lista,sizeof(int),1,out); //escreve a cabeça da lista no cabeçalho
	}

	int tamanho_procurado = getTamanhoCompanhia(companhia); //é preciso saber o tamanho doregistro para encontrar a posição adequada
	printf("tamanho que vai ser inserido: %d\n",tamanho_procurado);

	//procura espaço para reutilização do espaco
	int curr_offset,prox_offset,tam_atual;
	int ant_offset = 0;
	fseek(out,0,SEEK_SET);
	fread(&curr_offset,sizeof(int),1,out);
	fseek(out,0,SEEK_SET);
	while(curr_offset != -1){
		fseek(out,curr_offset+sizeof(char),SEEK_SET); //+sizeof(char) para ignorar o '*',q n tem utilidade nessa parte do projeto
		fread(&tam_atual,sizeof(int),1,out);
		printf("tam atual %d procurando %d\n",tam_atual,tamanho_procurado);
		if(tam_atual >= tamanho_procurado){
			printf("coube!\n");
			//religa a lista
			fread(&prox_offset,sizeof(int),1,out);
			fseek(out,ant_offset,SEEK_SET);
			fwrite(&prox_offset,sizeof(int),1,out);
			//sai do laço pq ja encontrou
			break;
		}
		ant_offset = curr_offset + sizeof(int) + sizeof(char); //soma valores para acesso direto ao byte q se encontra o offset
		fread(&curr_offset,sizeof(int),1,out);
	}
	//posiciona o ponteiro onde o registro deve ser inserido
	if(curr_offset != -1) fseek(out,curr_offset,SEEK_SET);
	else fseek(out,0,SEEK_END);


	int offset = (int) ftell(out);
	char cnpj_nulo[TAMANHO_CNPJ] = CNPJ_NULO;
	char data_nula[TAMANHO_DATA] = DATA_NULA;

	//escreve os campos de tamanho fixo tratando o nulo

	if(companhia->cnpj) fwrite(companhia->cnpj,sizeof(char),strlen(companhia->cnpj)+1,out);
	else fwrite(cnpj_nulo,sizeof(char),strlen(cnpj_nulo)+1,out);
		
	if(companhia->cnpj_auditoria) fwrite(companhia->cnpj_auditoria,sizeof(char),strlen(companhia->cnpj_auditoria)+1,out);
	else fwrite(cnpj_nulo,sizeof(char),strlen(cnpj_nulo)+1,out);
	
	if(companhia->data_registro) fwrite(companhia->data_registro,sizeof(char),strlen(companhia->data_registro)+1,out);
	else fwrite(data_nula,sizeof(char),strlen(data_nula)+1,out);
	
	if(companhia->data_cancelamento) fwrite(companhia->data_cancelamento,sizeof(char),strlen(companhia->data_cancelamento)+1,out);
	else fwrite(data_nula,sizeof(char),strlen(data_nula)+1,out);

	//escreve cada campo variavel e um delimitador entre eles
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

	fclose(out);

	return offset;
}

void firstFit(FILE *fp, int offset, int tamanho){
	char rem = '*';

	//a cabeça da lista vai passar a ser o proximo elemento
	int next_elem; //o proximo elemento na lista ligada
	fseek(fp,0,SEEK_SET); //posicao do regitro de cabeçalho
	fread(&next_elem,sizeof(int),1,fp);
	
	//a nova cabeça da lista é o elemento sendo removido agora
	fseek(fp,0,SEEK_SET);
	fwrite(&offset,sizeof(int),1,fp);

	//o registro é removido logicamente, escrevendo '*',tamanho do registro e prox na lista de removidos
	fseek(fp,offset,SEEK_SET);
	fwrite(&rem,sizeof(char),1,fp); //escreve *
	fwrite(&tamanho,sizeof(int),1,fp); //escreve o tamanho do registro
	fwrite(&next_elem,sizeof(int),1,fp); //escreve o offset do prox elemento na lista
}


void worstFit(FILE *fp,int offset,int tamanho){
	char rem = '*';
	int end_offset_prox = 0;//porque a primeiro valor está no cabeçalho do arquivo
	int offset_prox;
	int tam_prox;
	while(1){
		fseek(fp,end_offset_prox,SEEK_SET);
		fread(&offset_prox,sizeof(int),1,fp);

		if(offset_prox != -1){
			fseek(fp,offset_prox+sizeof(char),SEEK_SET);//+sizeof(char) pra pular o byte de romovido('*')
			fread(&tam_prox,sizeof(int),1,fp);

			if(tam_prox > tamanho){//se o tamanho do elemento for menor continua andando na lista
				end_offset_prox = ftell(fp);	
				continue; 
			}
		}	
		fseek(fp,end_offset_prox,SEEK_SET); //proximo do anterior passa a ser o novo
		fwrite(&offset,sizeof(int),1,fp);

		fseek(fp,offset,SEEK_SET);
		fwrite(&rem,sizeof(char),1,fp); //escreve *
		fwrite(&tamanho,sizeof(int),1,fp); //escreve o tamanho do registro
		fwrite(&offset_prox,sizeof(int),1,fp); //proximo do novo passa a ser o proximo do anterior

		break;
	}
}

void bestFit(FILE *fp,int offset,int tamannho){
	char rem = '*';
	int end_offset_prox = 0;//porque a primeiro valor está no cabeçalho do arquivo
	int offset_prox;
	int tam_prox;
	while(1){
		fseek(fp,end_offset_prox,SEEK_SET);
		fread(&offset_prox,sizeof(int),1,fp);

		if(offset_prox != -1){
			fseek(fp,offset_prox+sizeof(char),SEEK_SET);//+sizeof(char) pra pular o byte de romovido('*')
			fread(&tam_prox,sizeof(int),1,fp);

			if(tam_prox < tamannho){//se o tamanho do elemento for menor continua andando na lista
				end_offset_prox = ftell(fp);	
				continue; 
			}
		}	
		fseek(fp,end_offset_prox,SEEK_SET); //proximo do anterior passa a ser o novo
		fwrite(&offset,sizeof(int),1,fp);

		fseek(fp,offset,SEEK_SET);
		fwrite(&rem,sizeof(char),1,fp); //escreve *
		fwrite(&tamannho,sizeof(int),1,fp); //escreve o tamanho do registro
		fwrite(&offset_prox,sizeof(int),1,fp); //proximo do novo passa a ser o proximo do anterior

		break;
	}
}

void removerRegistro(FILE *out, int offset){

	fseek(out,offset,SEEK_SET);

	//pega o tamanho do registro que vai ser removido
	char c = 0;
	int tamanho = 0;
	while(c != DELIM_FIM_REG){
		c = fgetc(out);
		tamanho++;
	}

}

/* Imprime todos os registros de um arquivo */
void imprimirTodos(FILE *in){
	Companhia *companhia;
	int end;

	fseek(in,0,SEEK_END);
	end = (int) ftell(in);
	fseek(in,4,SEEK_SET); //pula o registro de cabeçalho
 
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
	char *buffer;


	for(int i=0;i<2;i++){ //le os dois campos cnpj
		buffer = (char*) malloc(sizeof(char) * (TAMANHO_CNPJ));
		fread(buffer,sizeof(char),TAMANHO_CNPJ,in);
		if(!strcmp(buffer,CNPJ_NULO)){ //caso o campo seja nulo
			free(buffer);
			buffer = NULL;
		}	
		salvarCampoCompanhia(companhia,campo++,buffer);
	}

	for(int i=0;i<2;i++){ //le os dois campos de data
		buffer = (char*) malloc(sizeof(char) * (TAMANHO_DATA));
		fread(buffer,sizeof(char),TAMANHO_DATA,in);
		if(!strcmp(buffer,DATA_NULA)){ //caso o campo seja nulo
			free(buffer);
			buffer = NULL;
		}	
		salvarCampoCompanhia(companhia,campo++,buffer);
	}

	while(run){
		fread(&c,sizeof(char),1,in);
		if(c == FLAG_REMOVIDO){
			companhia = NULL;
			run = 0;
			do{ //ignora o resto dos caracteres até chegar no fim do regitro
				fread(&c,sizeof(char),1,in);
			}while(c != DELIM_FIM_REG);
		}else if(c == DELIM_FIM_CAMPO){
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