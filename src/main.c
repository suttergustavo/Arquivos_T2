#include <stdlib.h>
#include <stdio.h>
#include <companhias.h>
#include <manipulacao_arquivos.h>

int main(int argc, char *argv[]){
	Companhia **cs,*c;
	int n_regs;

	cs = lerCSVCompleto("in",&n_regs);
	for(int i=0;i<n_regs;i++)
		escreverCompanhia("out",cs[i],DELIMITADOR_REGISTROS);
	for(int i=0;i<n_regs;i++)
		destruirCompanhia(cs[i]);
	free(cs);

	cs = buscarPorCampo("out",NOME_SOCIAL,"521 PARTICIPAÇOES S.A. - EM LIQUIDAÇÃO EXTRAJUDICIAL",&n_regs,DELIMITADOR_REGISTROS);
	for(int i=0;i<n_regs;++i){
		imprimirCompanhia(cs[i]);
		destruirCompanhia(cs[i]);
	}
	free(cs);

	c = buscarPorPosicao("out",2,DELIMITADOR_REGISTROS);
	if(c)imprimirCompanhia(c);
	else printf("nao\n");
	if(c)destruirCompanhia(c);

	cs = lerTodasCompanhias("out",&n_regs, DELIMITADOR_REGISTROS);
	for(int i=0; i<n_regs; ++i){
		imprimirCompanhia(cs[i]);
		printf("------------\n");
		destruirCompanhia(cs[i]);
	}
	free(cs);
}