#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <companhias.h>
#include <registro_delimitador.h>
#include <indice.h>


/* Desaloca o RegIndice da memoria*/
void destruirRegIndice(RegIndice *reg){
	free(reg->cnpj);
	free(reg);
}

/* Escreve o indice no disco*/
void escreverIndice(FILE *indice,RegIndice *registro_indice) {
	fwrite(registro_indice->cnpj,sizeof(char),TAMANHO_CNPJ,indice);
	fwrite(&(registro_indice->offset),sizeof(int),1,indice);
}

/* Tranfere para o arquivo no disco as informações nas estruturas na memoria */
void salvarIndice(char *filename, Indice *in) {
	FILE *fp = fopen(filename,"w+");
	int validade = INVALIDO;
	
	fwrite(&validade, sizeof(int), 1, fp); //antes de tranferir tudo o indice ainda é invalido
	for (int i = 0; i < in->size; i++)
		escreverIndice(fp, in->indice[i]);
		
	//quando acaba de escrever avisa que o conteudo do arquvo é valido
	validade = VALIDO;
	fseek(fp, 0, SEEK_SET);
	fwrite(&validade, sizeof(int), 1, fp);

	fclose(fp);
}


/*Leitura do indice individual do disco para memoria principal*/
RegIndice *lerIndice(FILE *indice){
	RegIndice *reg = (RegIndice*) malloc(sizeof(RegIndice));
	reg->cnpj = (char*) malloc(sizeof(char) * TAMANHO_CNPJ);

	fread(reg->cnpj,sizeof(char),TAMANHO_CNPJ,indice);
	fread(&reg->offset,sizeof(int),1,indice);

	return reg;
}

/* Recupera um indice corrompido usando o arquivo de dados */
Indice *recuperarIndice(char *filename) {
	FILE *fp = fopen(filename, "r");
	Indice *indice = criarIndice();
	RegIndice *reg;

	fseek(fp, 0, SEEK_END);
	int size = (int) ftell(fp);	
	fseek(fp, 4, SEEK_SET);

	//itera no arquivo de dados obtendo o indice
	while (ftell(fp) < size) {
		reg = lerCompanhiaRecuperacao(fp);
		if(reg != NULL){
			inserirIndice(indice,reg->cnpj,reg->offset);
			destruirRegIndice(reg);
		} 
	}

	fclose(fp);
	return indice;
}

/* Tranfere um indice do disco para memoria, ou cria caso não exista*/
Indice *carregarIndice(char *filename, char *dataname) {
	Indice *indice = criarIndice();
	int count = 0, validade;

	FILE *fp = fopen(filename,"r");

	if(fp == NULL){	//se não existe arquivo é porque o projeto é novo
		printf("Arquivo de indice inexistente, será criado\n");
		fp = fopen(filename,"w+"); //ja cria o arquivo para posteriomente ser usado(quando o indice for alterado será alterado o buty de validade)
		fclose(fp);
		return indice;	
	}

	fseek(fp, 0, SEEK_SET);
	fread(&validade, sizeof(int), 1, fp);
	if(validade == INVALIDO){ //se o byte de validade indicar que o arquivo está corrompido
		printf("Arquivo de indice corrompido. Iniciando recuperação...\n");
		destruirIndice(indice);//libera o indice de antes, ja que outro será criado
		indice = recuperarIndice(dataname);
		printf("Indice recuperado com sucesso!\n");
		fclose(fp);
		return indice;
	}


	fseek(fp, 0, SEEK_END);
	int size = (int) ftell(fp);	

	fseek(fp, 4, SEEK_SET);
	while(size > (int) ftell(fp)){
		indice->indice = (RegIndice **) realloc(indice->indice, sizeof(RegIndice *) * (count+1));
		indice->indice[count++] = lerIndice(fp);
	}
	indice->size = count;

	printf("Arquivo de indice carregado com sucesso\n");

	fclose(fp);

	return indice;
}

/* Realiza a busca por um CNPJ no indice */
int buscarIndice(Indice *in, char *cnpj) {
	int pos,start = 0, end = in->size - 1;

	while (start <= end) {
		pos = (start + end) / 2;
		if (!strcmp(cnpj, in->indice[pos]->cnpj)) return pos;
		else if (strcmp(cnpj, in->indice[pos]->cnpj) < 0) end = pos-1;
		else start = pos+1;
	}

	return -1; // nao encontrado

}

/* Seta o byte de validade no cabeçalho do arquivo */
void validadeIndice(char *filename, Validade validade) {
	FILE *indice = fopen(filename,"r+");

	fseek(indice, 0, SEEK_SET);
	fwrite(&validade, sizeof(int), 1, indice);

	fclose(indice);
}

/* Função de comparação usada pelo qsort */
int compararRegIndice(const void *a, const void *b){
	RegIndice *ra = *(RegIndice **)a;
	RegIndice *rb = *(RegIndice **)b;
	return strcmp(ra->cnpj,rb->cnpj);
}

/*Insere novo indice no vetor de indices*/
void inserirIndice(Indice *in ,char *cnpj,int offset) {
	RegIndice *reg = (RegIndice*) malloc(sizeof(RegIndice));
	reg->cnpj = (char *) malloc(sizeof(char)*TAMANHO_CNPJ);
	strcpy(reg->cnpj,cnpj);
	reg->offset = offset;

	in->indice = (RegIndice **) realloc(in->indice, sizeof(RegIndice *) * (in->size + 1));
	in->indice[in->size++] = reg;

	qsort(in->indice,in->size,sizeof(RegIndice*),compararRegIndice);
}


/* Remove um indice e retorna offset do registro */
int removerIndice(Indice *in, char *cnpj) {
	int pos = buscarIndice(in, cnpj); //procura pelo indice do registro que será removido

	if(pos == -1) return -1; //nao encontrado portanto não pode ser removido

	int offset = in->indice[pos]->offset;
	destruirRegIndice(in->indice[pos]); //tira o indice da memoria

	while(pos != in->size-1){ //shifta o resto do vetor
		in->indice[pos] = in->indice[pos+1];
		pos++;
	}

	in->size--; //reduz o tamanho do vetor de indices

	return offset; //removido com sucesso
}

/* Desaloca o indice da memoria */
void destruirIndice(Indice *in) {
	for (int i = 0; i < in->size; i++)
		destruirRegIndice(in->indice[i]);

	free(in->indice);
	free(in);
}

/* Aloca uma estrutura indice */
Indice *criarIndice(){
	return (Indice *) calloc(1,sizeof(Indice));
}