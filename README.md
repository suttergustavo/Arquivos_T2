## Informações básicas do trabalho ##
* Os registros possuem registros tanto de tamanho fixo quanto de tamanho variável.
* Teremos que implementar 3 tipos de registros(todos de tamanho variável):
     * Indicador de tamanho pra tamanho do registro  
     * Delimitador de final de Registro 
     * Número fixo de campos

* Nosso código deve realizar as seguintes operações:
     * Ler arquivo de entrada CSV e guardar dados em arquivo
     * Imprimir todos os registros de um arquivo
     * Buscar registro por um *Nome Fantasia*
     * Busca registro por posição
     * Buscar por determinado campo de um arquivo de certa posição

* Campos de Tamanho Fixo:
     * CNPJ
     * CNPJ Auditoria
     * Data Registro
     * Data Cancelamento

* Campos de Tamanho Variável:
     * Nome Fantasia
     * Nome Social
     * Motivo Cancelamento
     * Nome Empresa


## Informações referentes ao código ##

### Conteúdo arquivos ###

* main.c - Menu de opções e chamada das funções que funcionam por trás
* companhia.c(.h) - Funções que trabalham com companhias, independente do formato de armazenamento nos arquivos.
* registro_[ind_tamanho,delimitador,numero_fixo].c(.h) - Funções que fazer a leitura e a escrita nos arquivos de acordo com o tipo de registro escolhido pelo usuário.

### Convenções de retorno ###

* Toda função de busca e impressão retorna um ponteiro para um elemento(struct Companhia), ao invés de imprimir no função.

### Convenções de Código ###

```
#!c

nome_variavel //nome variável

NomeStruct //nome struct

nomeFuncao() //nome função

NOME_DEFINE //nome define
```