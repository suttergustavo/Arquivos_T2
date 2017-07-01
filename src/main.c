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

// void testeBuscaIndice() {
// 	char *cnpj;
// 	Companhia *c;

// 	FILE *in = fopen("out","r+");
// 	FILE *indice = fopen("indice.dat","r");

// 	printf ("Digite uma busca por CNPJ\n");
// 	scanf ("%ms", &cnpj);

// 	/////////

// 	if (c) imprimirCompanhia(c);
// 	else printf ("NÃO ENCONTRADO");
	
// }

void printIndice(Indice *idx){
	for(int i=0;i<idx->size;i++){
		printf("%s %d\n",idx->indice[i]->cnpj,idx->indice[i]->offset);
	}
}

int main(int argc, char *argv[]){
	int op;
	Indice *indice = criarIndice();
	while(scanf("%d",&op) && op){
		if(op == 1) csv2dados(indice);
		if(op == 2) removerCompanhia(indice);
		if(op == 3) dados2tela(indice);
		if(op == 4) indice2tela();
		// if(op == 5) testeBuscaIndice();
		if(op == 6) printIndice(indice);
	}
}	