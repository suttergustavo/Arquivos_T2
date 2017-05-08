/*
Autores:

Gustavo Sutter - 9763193
Matheus Gomes - 9779270
Guilherme Montemovo - 9779461
Joice Aurino - 8530851

*/

#ifndef _MANIIPULACAO_ARQUIVOS_H_
#define _MANIIPULACAO_ARQUIVOS_H_

#include <companhias.h>

typedef enum{
	INDICADOR_TAMANHO = 1,
	DELIMITADOR_REGISTROS,
	NUMERO_FIXO_CAMPOS
} Metodo;

/*Escreve companhia no arquivo indicado usando o metodo passado*/
void escreverCompanhia(char *, Companhia *, Metodo);
/*Busca no arquivo por companhia usando um determinado campo usando o metodo passado*/
Companhia **buscarPorCampo(char *,Campo,char*,int*,Metodo);
/*Busca no arquivo por companhia usando sua posição usando o metodo passado*/
Companhia *buscarPorPosicao(char *,int,Metodo);
/* Le todas os companhias do arquivo usando o metodo passado*/
Companhia **lerTodasCompanhias(char*,int*,Metodo);

#endif