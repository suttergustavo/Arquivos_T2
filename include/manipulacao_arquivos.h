#ifndef _MANIIPULACAO_ARQUIVOS_H_
#define _MANIIPULACAO_ARQUIVOS_H_

#include <companhias.h>

typedef enum{
	INDICADOR_TAMANHO,
	NUMERO_FIXO_CAMPOS,
	DELIMITADOR_REGISTROS
} Metodo;

void escreverCompanhia(char *, Companhia *, Metodo);
Companhia **buscarPorCampo(char *,Campo,char*,int*,Metodo);
Companhia *buscarPorPosicao(char *,int,Metodo);
Companhia **lerTodasCompanhias(char*,int*,Metodo);

#endif