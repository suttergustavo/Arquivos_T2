#include <stdlib.h>
#include <stdio.h>
#include <companhias.h>
#include <registro_num_fixo_campos.h>
#include <registro_delimitador.h>
#include <registro_indicador_tamanho.h>

int main(int argc, char *argv[]){
	FILE *output = fopen("out","ab+");
	Companhia *c,**cs,**s;
	int n_regs;

	cs = lerCSVCompleto("in",&n_regs);
	for(int i=0;i<n_regs;i++)
		escreverCompanhiaTamReg(output,cs[i]);
	for(int i=0;i<n_regs;i++)
		destruirCompanhia(cs[i]);
	free(cs);

	s = lerTodosTamReg(output,&n_regs);
	for (int i = 0; i < n_regs; ++i){
		imprimirCompanhia(s[i]);
		printf("--------------\n");
	}
	for(int i=0;i<n_regs;i++)
		destruirCompanhia(s[i]);
	free(s);



	c = buscarNumRegTamReg(output,2);
	if(c){
	 	imprimirCompanhia(c);
	 	destruirCompanhia(c);
	}
	else printf("Não Encontrado!\n");
	
	int count;
	s = buscarCampoTamReg(output, NOME_SOCIAL, "521 PARTICIPAÇOES S.A. - EM LIQUIDAÇÃO EXTRAJUDICIAL",&count);
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