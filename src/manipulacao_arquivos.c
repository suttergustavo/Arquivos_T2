#include <stdlib.h>
#include <stdio.h>
#include <manipulacao_arquivos.h>
#include <registro_delimitador.h>
#include <registro_indicador_tamanho.h>
#include <registro_num_fixo_campos.h>


/*Escreve companhia no arquivo indicado usando o metodo passado*/
void escreverCompanhia(char *nome_arquivo, Companhia *companhia, Metodo metodo){
	FILE *fp;

	fp = fopen(nome_arquivo,"ab+");

	if(metodo == INDICADOR_TAMANHO){
		escreverCompanhiaTamReg(fp,companhia);
	}else if(metodo == NUMERO_FIXO_CAMPOS){
		escreverCompanhiaNumFixo(fp,companhia);
	}else if(metodo == DELIMITADOR_REGISTROS){
		escreverCompanhiaDelimitador(fp,companhia);
	}

	fclose(fp);
}


/*Busca no arquivo por companhia usando um determinado campo usando o metodo passado*/
Companhia **buscarPorCampo(char *nome_arquivo, Campo campo, char *query, int *n_regs, Metodo metodo){
	FILE *fp;
	Companhia **companhias;

	fp = fopen(nome_arquivo,"rb");

	if(fp != NULL){
		if(metodo == INDICADOR_TAMANHO){
			companhias = buscarCampoTamReg(fp,campo,query,n_regs);
		}else if(metodo == NUMERO_FIXO_CAMPOS){
			companhias = buscarCampoNumFixo(fp,campo,query,n_regs);
		}else if(metodo == DELIMITADOR_REGISTROS){
			companhias = buscarCampoDelimitador(fp,campo,query,n_regs);
		}
		fclose(fp);
	}

	return companhias;
}


/*Busca no arquivo por companhia usando sua posição usando o metodo passado*/
Companhia *buscarPorPosicao(char *nome_arquivo, int posicao, Metodo metodo){
	FILE *fp;
	Companhia *companhia;

	fp = fopen(nome_arquivo,"rb");

	if(fp != NULL){
		if(metodo == INDICADOR_TAMANHO){
			companhia = buscarNumRegTamReg(fp,posicao);
		}else if(metodo == NUMERO_FIXO_CAMPOS){
			companhia = buscarNumRegNumFixo(fp,posicao);
		}else if(metodo == DELIMITADOR_REGISTROS){
			companhia = buscarNumRegDelimitador(fp,posicao);
		}
		fclose(fp);
	}

	return companhia;
}


/* Le todas os compnhias do arquivo usando o metodo passado*/
Companhia **lerTodasCompanhias(char* nome_arquivo, int *n_regs, Metodo metodo){
	FILE *fp = NULL;
	Companhia **companhias;

	fp = fopen(nome_arquivo,"rb");

	if(fp != NULL){
		if(metodo == INDICADOR_TAMANHO){
			companhias = lerTodosTamReg(fp,n_regs);
		}else if(metodo == NUMERO_FIXO_CAMPOS){
			companhias = lerTodosNumFixo(fp,n_regs);
		}else if(metodo == DELIMITADOR_REGISTROS){
			companhias = lerTodosDelimitador(fp,n_regs);
		}
		fclose(fp);
	}

	return companhias;	
}