#include <stdlib.h>
#include <stdio.h>
#include <companhias.h>
#include <registro_delimitador.h>

int main(int argc, char *argv[]){
	FILE *output = fopen("out","ab+");
	Companhia *c,**cs,**s;
	int n_regs;

	cs = lerCSVCompleto("in",&n_regs);
	for(int i=0;i<n_regs;i++)
		escreverCompanhiaDelimitador(output,cs[i]);
	for(int i=0;i<n_regs;i++)
		destruirCompanhia(cs[i]);
	free(cs);

	c = buscarNomeDelimitador(output,"ABC COMPUTADORES");
	if(c){
		imprimirCompanhia(c);
		destruirCompanhia(c);
	}
	else printf("Não Encontrado!\n");

	c = buscarNumRegDelimitador(output,1);
	if(c){
		imprimirCompanhia(c);
		destruirCompanhia(c);
	}
	else printf("Não Encontrado!\n");
	int count;
	//s = lerTodosDelimitador(output,&count);
	s = buscarCampoDelimitador(output, NOME_EMPRESA, "GRANT THORNTON AUDITORES INDEPENDENTES",&count);
	printf("%d\n",count);
	for(int i=0;i<count;i++){
		printf("--------------------\n");
		imprimirCompanhia(s[i]);
		printf("--------------------\n");
	}
	for(int i=0;i<count;i++)
		destruirCompanhia(s[i]);
	free(s);


	fclose(output);

}