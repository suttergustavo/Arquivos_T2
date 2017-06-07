#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <companhias.h>
#include <indice.h>

void escreverIndice(FILE *indice,char *cnpj,int offset){
	fwrite(cnpj,sizeof(char),TAMANHO_CNPJ + 1,indice);
	fwrite(&offset,sizeof(int),1,indice);
}

RegIndice *lerIndice(FILE *indice){
	RegIndice *reg = (RegIndice*) malloc(sizeof(RegIndice));
	reg->cnpj = (char*) malloc(sizeof(char) * TAMANHO_CNPJ + 1);

	fread(reg->cnpj,sizeof(char),TAMANHO_CNPJ + 1,indice);
	fread(&reg->offset,sizeof(int),1,indice);


	return reg;
}