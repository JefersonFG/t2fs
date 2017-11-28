/**
 * Funções de manipulação de arquivos e diretórios.
 */

#include "t2fs.h"

#ifndef T2FS_FILE_OPERATIONS_H
#define T2FS_FILE_OPERATIONS_H

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

#endif //T2FS_FILE_OPERATIONS_H
