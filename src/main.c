#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <companhias.h>
#include <registro_delimitador.h>
#include <indice.h>

void csv2dados(Indice *indice){
	FILE *in = fopen("in","r");
	Companhia *c;
	char out[4] = "out";

	fseek(in,0,SEEK_END);
	int size = (int) ftell(in);	
	fseek(in,0,SEEK_SET);
	
	while(size > (int) ftell(in)){
		c = lerCompanhiaCSV(in);
		imprimirCompanhia(c);
		int o = escreverCompanhia(out,c);
		inserirIndice(indice,c->cnpj,o);
		printf("offset = %d\n",o);
		printf("-----------\n");
	}
}

void dados2tela(Indice *idx){
	imprimirTodos("out",idx);
}

void indice2tela(){
	FILE *in = fopen("indice.dat","r");
	imprimirIndice(in);
}

void removerCompanhia(Indice *idx){
	removerRegistro("out",4);
	removerRegistro("out",175);
	removerIndice(idx,"92.659.614/0001-06");
	removerIndice(idx,"01.851.771/0001-55");
}

void printIndice(Indice *idx){
	for(int i=0;i<idx->size;i++){
		printf("%s %d\n",idx->indice[i]->cnpj,idx->indice[i]->offset);
	}
}

int main(int argc, char *argv[]){
	int op;
	char *idx_file;
	scanf("%ms",&idx_file);
	Indice *indice = carregarIndice(idx_file);
	while(scanf("%d",&op) && op){
		if(op == 1) csv2dados(indice);
		if(op == 2) removerCompanhia(indice);
		if(op == 3) dados2tela(indice);
		if(op == 4) salvarIndice(idx_file,indice);
		if(op == 6) printIndice(indice);
	}
}	