/*
Autores:

Gustavo Sutter - 9763193
Matheus Gomes - 9779270
Guilherme Montemovo - 9779461
Joice Aurino - 8530851

*/

#ifndef _REGISTRO_DELIMITADOR_H_
#define _REGISTRO_DELIMITADOR_H_

#include <companhias.h>
#include <indice.h>

//estrategias de reuso do espaço
typedef enum {
	FIRST_FIT,
	BEST_FIT,
	WORST_FIT
}Estrategia;

/* Escreve companhia no arquivo de dados e retorna o byte offset */
int escreverCompanhia(char*,Companhia*,Estrategia);
/* Lê um registro de um arquivo de dados, a partir posição desse registro */
Companhia *lerCompanhia(char *, int);
/* Remove logicamente um registro e o coloca na lista de removidos*/
void removerRegistro(char*,int,Estrategia);
/* Imprime a lista de reuso de espaço que esta no arquivo de dados*/
void imprimirListaRemovidos(char *,char *);
/* Função de leitura usada na recuperação de indices corrompidos */
RegIndice *lerCompanhiaRecuperacao(FILE *);

#endif