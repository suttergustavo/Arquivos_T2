#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <companhias.h>
#include <registro_delimitador.h>
#include <indice.h>

void csv2dados(){
	FILE *in = fopen("in","r");
	FILE *indice = fopen("indice.dat","a");
	FILE *out = fopen("out","ab+");
	//FILE *indice = fopen("indice.dat","w");
	Companhia *c;
	char out[4] = "out";

	fseek(in,0,SEEK_END);
	int size = (int) ftell(in);	
	fseek(in,0,SEEK_SET);
	
	while(size > (int) ftell(in)){
		c = lerCompanhiaCSV(in);
		imprimirCompanhia(c);
		int o = escreverCompanhia("out",c);
		escreverIndice(indice,c->cnpj,o);
		printf("offset = %d\n",o);
		printf("-----------\n");
	}
}

void dados2tela(){
	FILE *in = fopen("out","r");
	imprimirTodos(in);
}

void indice2tela(){
	FILE *in = fopen("indice.dat","r");
	imprimirIndice(in);

}

void removerCompanhia(){
	FILE *in = fopen("out","r+");
	removerRegistro(in,4);
	removerRegistro(in,175);
}

void testeBuscaIndice() {
	/*char *cnpj;
	Companhia *c;

	FILE *in = fopen("out","r+");
	FILE *indice = fopen("indice.dat","r");

	printf ("Digite uma busca por CNPJ\n");
	scanf ("%ms", &cnpj);

	/////////

	if (c) imprimirCompanhia(c);
	else printf ("NÃO ENCONTRADO");
	*/
}

int main(int argc, char *argv[]){
	int op;
	scanf("%d",&op);
	if(op == 1) csv2dados();
	if(op == 2) removerCompanhia();
	if(op == 3) dados2tela();
	if(op == 4) indice2tela();
	if(op == 5) testeBuscaIndice();

}	