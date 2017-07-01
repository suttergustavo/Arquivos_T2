#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <companhias.h>
#include <registro_delimitador.h>
#include <indice.h>

// Escreve no disco o indice 
void escreverIndice(FILE *indice,RegIndice *registro_indice) {
	fwrite(registro_indice->cnpj,sizeof(char),TAMANHO_CNPJ + 1,indice);
	fwrite(&(registro_indice->offset),sizeof(int),1,indice);
}

// Cria arquivo de indices a partir de um vetor na memoria
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
}


// Leitura do indice individual do disco para memoria principal
RegIndice *lerIndice(FILE *indice){
	RegIndice *reg = (RegIndice*) malloc(sizeof(RegIndice));
	reg->cnpj = (char*) malloc(sizeof(char) * TAMANHO_CNPJ + 1);

	fread(reg->cnpj,sizeof(char),TAMANHO_CNPJ + 1,indice);
	fread(&reg->offset,sizeof(int),1,indice);

	return reg;
}

// Cria arquivo de indices completo na memoria a partir do indice no disco
Indice *carregarIndice(char *filename) {
	Indice *indice = criarIndice();
	int count = 0, validade;

	FILE *fp = fopen(filename,"r");

	if(fp == NULL){	//se não existe arquivo é porque o projeto é novo
		printf("Arquivo inexistente, será criado\n");
		fopen(filename,"w+"); //ja cria o arquivo para posteriomente ser usado(quando o indice for alterado será alterado o buty de validade)
		return indice;	
	}

	fseek(fp, 0, SEEK_SET);
	fread(&validade, sizeof(int), 1, fp);
	if(validade == INVALIDO){ //se o byte de validade indicar que o arquivo está corrompido
		printf("Arquivo de indice corrompido. Iniciando recuperação...\n");
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

	return indice;
}

int buscarIndice(Indice *in, char *cnpj) {
	// int pos = in->size/2, start = 0, end = in->size - 1;

	// while (start < end) {
	// 	if (!strcmp(cnpj, in->indice[pos]->cnpj)) return pos;
	// 	else if (strcmp(cnpj, in->indice[pos]->cnpj) < 0) end = pos-1;
	// 	else start = pos+1;
	// }

	// return -1; // nao encontrado

	int pos = -1;
	for(int i=0;i<in->size;i++){
		if(!strcmp(in->indice[i]->cnpj,cnpj)){
			pos = i;
			break;
		}
	}

	return pos;
}

// Coloca o indice como valido ou invalido. Deve ser utilizado quando ocorrer uma inserçao, remoçao, atualização, ou quando o programa encerrar
void validadeIndice(FILE *indice, Validade validade) {
	fseek(indice, 0, SEEK_SET);
	fwrite(&validade, sizeof(int), 1, indice);
}


// Insere no arquivo de indices da memoria
void inserirIndice(Indice *in ,char *cnpj,int offset) {
	RegIndice *reg = (RegIndice*) malloc(sizeof(RegIndice));
	reg->cnpj = cnpj;
	reg->offset = offset;

	in->indice = (RegIndice **) realloc(in->indice, sizeof(RegIndice *) * (in->size + 1));
	in->indice[in->size++] = reg;
}

void destruirRegIndice(RegIndice *reg){
	if(reg->cnpj) free(reg->cnpj);
	free(reg);
}

// Remove um indice
int removerIndice(Indice *in, char *cnpj) {
	int pos = buscarIndice(in, cnpj); //procura pelo indice do registro que será removido

	if(pos == -1) return 0; //nao encontrado portanto não pode ser removido

	destruirRegIndice(in->indice[pos]); //tira o indice da memoria

	while(pos != in->size-1){ //shifta o resto do vetor
		in->indice[pos] = in->indice[pos+1];
		pos++;
	}

	in->size--; //reduz o tamanho do vetor de indices

	return 1; //removido com sucesso
}

void imprimirIndice(FILE *indice) {
	RegIndice *r;
	
	fseek(indice,0,SEEK_END);
	int size = (int) ftell(indice);	
	fseek(indice,0,SEEK_SET);
	
	while(size > (int) ftell(indice)){
		r = lerIndice(indice);
		printf("%s | %d\n",r->cnpj,r->offset);
		destruirRegIndice(r);
	}
}

void destruirIndice(Indice *in) {
	for (int i = 0; i < in->size; i++)
		destruirRegIndice(in->indice[i]);

	free(in->indice);
	free(in);
}

Indice *criarIndice(){
	return (Indice *) calloc(1,sizeof(Indice));
}