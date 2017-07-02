#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <companhias.h>
#include <registro_delimitador.h>
#include <indice.h>
#include <projeto.h>

void csv2dados(){
	
}

void dados2tela(Indice *idx){
	imprimirTodos("out",idx);
}

void indice2tela(){
	FILE *in = fopen("indice.dat","r");
	imprimirIndice(in);
}

void printIndice(Indice *idx){
	for(int i=0;i<idx->size;i++){
		printf("%s %d\n",idx->indice[i]->cnpj,idx->indice[i]->offset);
	}
}

void teste(){

}

int main(int argc, char *argv[]){
	int op;
	Projeto *p = iniciarProjeto("xurbo");
	while(scanf("%d",&op) && op){
		if(op == 1) inserirDoCSV(p,"in");
		if(op == 2){
			removerCompanhia(p,"92.659.614/0001-06");
			removerCompanhia(p,"01.851.771/0001-55");
		}
		if(op == 3) imprimirIndices(p);
		if(op == 4){
			Companhia *c = lerCompanhiaIndividual();
			inserirCompanhiaIndividual(p,c);
		}
		if(op == 5) imprimirListas(p);
	}
	salvarIndices(p);
}	