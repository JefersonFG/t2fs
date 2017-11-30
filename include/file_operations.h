/**
 * Funções de manipulação de arquivos e diretórios.
 */

#ifndef T2FS_FILE_OPERATIONS_H
#define T2FS_FILE_OPERATIONS_H

#include "t2fs.h"
#include "fsmanager.h"

/**
 * Lê a lista de entradas do diretório passado.
 * @param diretorio Diretório do qual ler a lista de entradas.
 * @param entradas_diretorio Lista de entradas do diretório passado.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int get_directory_entries(struct t2fs_record *diretorio, struct t2fs_record *entradas_diretorio);

/**
 * Retorna a lista de entradas do diretório pai à partir do caminho passado. Essa função considera que o
 * caminho passado inclui somente diretórios existentes e retorna o registro correspondente
 * ao último diretório desse caminho. Se algum diretório do caminho não existir retorna um erro.
 * @param pathname Caminho cujo último elemento é o diretório pai.
 * @param entradas_diretorio_pai Lista de elementos correspondente ao diretório pai.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int get_father_dir(char *pathname, struct t2fs_record* entradas_diretorio_pai);

/**
 * Cria um novo diretório sob o diretório pai passado e escreve no disco.
 * @param nome_diretorio Nome do novo diretório a ser criado.
 * @param diretorio_pai Diretório pai.
 * @param entradas_diretorio_pai Lista de registros do diretório pai.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int create_dir(char *nome_diretorio, struct t2fs_record *diretorio_pai, struct t2fs_record *entradas_diretorio_pai);
//#define FILE_HANDLE 1
//#define DIR_HANDLE 2

#define MAX_OPEN_FILES 10
#define MAX_OPEN_DIRS 25 //foda-se

//TODO Adicionar funções de manipulação de arquivos e diretórios.

/**
 * Gera um novo Handle do tipo desejado, contanto que o limite de
 * arquivos abertos simultaneamente seja respeitado.
 *
 * @param type 1 - arquivo; 2 - diretório
 * @return -1 em caso de erro; o Handle criado caso execute corretamente
 */
FILE2 getNewHandle(int type);
char* getFileName(char *filename);

#endif //T2FS_FILE_OPERATIONS_H
