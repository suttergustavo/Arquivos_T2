#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <companhias.h>
#include <registro_delimitador.h>
#include <indice.h>

// Cria o arquivo de indice no disco após a criação de todo arquivo de registros, também em disco
void criarIndice(FILE *indice, FILE *in) {
	Companhia *c;
	int offset;

	fseek(in, 0, SEEK_END);
	int size = (int) ftell(in);	
	fseek(in, 0, SEEK_SET);
	
	while(size > (int) ftell(in)){
		offset = ftell(in);
		c = lerCompanhia(in);
		escreverIndice(indice,c->cnpj,offset);

	}
}

// Escreve no disco o indice
void escreverIndice(FILE *indice,char *cnpj,int offset){
	fwrite(cnpj,sizeof(char),TAMANHO_CNPJ + 1,indice);
	fwrite(&offset,sizeof(int),1,indice);
}

int possuiIndiceProcurado(RegIndice *reg, char *query) {
	if(reg->cnpj && !strcmp(reg->cnpj,query)) return 1;
	else return 0;
}

void imprimirIndice(FILE *indice) {
	RegIndice *r;
	
	fseek(indice,0,SEEK_END);
	int size = (int) ftell(indice);	
	fseek(indice,0,SEEK_SET);
	
	while(size > (int) ftell(indice)){
		r = lerIndice(indice);
		printf("%s | %d\n",r->cnpj,r->offset);
		destruirIndice(r);
	}
}

void destruirIndice(RegIndice *reg){
	if(reg->cnpj) free(reg->cnpj);

	free(reg);
}

// Leitura do indice individual do disco para memoria principal
RegIndice *lerIndice(FILE *indice){
	RegIndice *reg = (RegIndice*) malloc(sizeof(RegIndice));
	reg->cnpj = (char*) malloc(sizeof(char) * TAMANHO_CNPJ + 1);

	fread(reg->cnpj,sizeof(char),TAMANHO_CNPJ + 1,indice);
	fread(&reg->offset,sizeof(int),1,indice);

	return reg;
}