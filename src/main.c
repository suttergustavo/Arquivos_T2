#include <stdlib.h>
#include <stdio.h>
#include <companhias.h>
#include <registro_num_fixo_campos.h>

int main(int argc, char *argv[]){
	FILE *output = fopen("out","ab+");
	Companhia *c,**cs,**s;
	int n_regs;

	cs = lerCSVCompleto("entrada.csv",&n_regs);
	for(int i=0;i<n_regs;i++)
		escreverCompanhiaNumFixo(output,cs[i]);
	for(int i=0;i<n_regs;i++)
		destruirCompanhia(cs[i]);
	free(cs);

	int count = 0;
	s = buscarCampoNumFixo(output,NOME_FANTASIA,"",&count);
	if(s != NULL){
		for(int i=0;i<count;i++){
			imprimirCompanhia(s[i]);
			printf("\n");
		}
	}else{
		printf("Nao Encontrado\n");
	}

	fclose(output);

}