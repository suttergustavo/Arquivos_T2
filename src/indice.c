#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <companhias.h>
#include <registro_delimitador.h>
#include <indice.h>

// Cria arquivo de indices a partir de um vetor na memoria
void salvarIndice(FILE *out, Indice *in) {
	int validade = VALIDO;

	fseek(out, 0, SEEK_SET);
	fwrite(&validade, sizeof(int), 1, out);
		
	for (int i = 0; i < in->size; i++){
		escreverIndice(out, in->indice[i]->cnpj, in->indice[i]->offset);
	}
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
Indice *carregarIndice(FILE *in) {
	Indice *out;
	RegIndice **indice = NULL;
	int count = 0, validade;

	fseek(in, 0, SEEK_SET);
	fread(&validade, sizeof(int), 1, in);
	if (validade == INVALIDO) return NULL;	// se o indice estiver desatualizado

	fseek(in, 0, SEEK_END);
	int size = (int) ftell(in);	
	fseek(in, 4, SEEK_SET);

	out = (Indice *)malloc(sizeof(Indice));
	
	while(size > (int) ftell(in)){
		indice = (RegIndice **)realloc(indice, sizeof(RegIndice *) * (count+1));
		indice[count++] = lerIndice(in);
	}
	out->indice = indice;
	out->size = count;

	return out;
}

int buscarIndice(Indice *in, char *cnpj) {
	int pos = in->size/2, start = 0, end = in->size - 1;

	while (start < end) {
		if (!strcmp(cnpj, in->indice[pos]->cnpj)) return pos;
		else if (strcmp(cnpj, in->indice[pos]->cnpj) < 0) end = pos-1;
		else start = pos+1;
	}

	return -1; // nao encontrado
}

// Coloca o indice como valido ou invalido. Deve ser utilizado quando ocorrer uma inserçao, remoçao, atualização, ou quando o programa encerrar
void validadeIndice(FILE *indice, Validade validade) {
	fseek(indice, 0, SEEK_SET);
	fwrite(&validade, sizeof(int), 1, indice);
}

// Escreve no disco o indice 
void escreverIndice(FILE *indice,char *cnpj,int offset) {
	fwrite(cnpj,sizeof(char),TAMANHO_CNPJ + 1,indice);
	fwrite(&offset,sizeof(int),1,indice);
}

// Insere no arquivo de indices da memoria
void inserirIndice(Indice *in ,char *cnpj,int offset) {
	RegIndice *reg = (RegIndice*) malloc(sizeof(RegIndice));
	reg->cnpj = cnpj;
	reg->offset = offset;

	in->indice = (RegIndice **)realloc(in, sizeof(RegIndice *) * (in->size));
	in->indice[in->size-1] = reg;

	in->size++;
}

// Remove um indice
int removerIndice(Indice *in, char *cnpj) {
	int pos = buscarIndice(in, cnpj);

	if (pos == -1) return -1; // nao encontrado

	destruirIndice(in->indice[pos]);

	while (pos != in->size-1) {
		in->indice[pos] = in->indice[pos+1];
		pos++;
	}

	in->size--;

	return 1;
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

void destruirIndiceCompleto(Indice *in) {
	for (int i = 0; i < in->size; i++)
		destruirIndice(in->indice[i]);

	free(in->indice);
	free(in);
}