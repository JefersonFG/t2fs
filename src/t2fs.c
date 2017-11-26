/**
 * Implementação da interface do t2fs.
 */

#include <string.h>
#include "../include/t2fs.h"
#include "../include/fsmanager.h"

#define IDENTIFICACAO_GRUPO "Jeferson Ferreira Guimaraes - 262522\n" \
"Luis Miguel Santos Batista - 265037\nVinicius Chagas Soares - 262510\n"

int identify2 (char *name, int size) {
    int tamanho_identificacao = strlen(IDENTIFICACAO_GRUPO);

    if (size < tamanho_identificacao)
        return -1;

    strcpy(name, IDENTIFICACAO_GRUPO);

    return 0;
}

FILE2 create2 (char *filename) {
    //TODO - Implementar create2
    return -1;
}

int delete2 (char *filename) {
    //TODO - Implementar delete2
    return -1;
}

FILE2 open2 (char *filename) {
    //TODO - Implementar open2
    return -1;
}

int close2 (FILE2 handle) {
    //TODO - Implementar close2
    return -1;
}

int read2 (FILE2 handle, char *buffer, int size) {
    //TODO - Implementar read2
    return -1;
}

int write2 (FILE2 handle, char *buffer, int size) {
    //TODO - Implementar write2
    return -1;
}

int truncate2 (FILE2 handle) {
    //TODO - Implementar truncate2
    return -1;
}

int seek2 (FILE2 handle, unsigned int offset) {
    //TODO - Implementar seek2
    return -1;
}

int mkdir2 (char *pathname) {
    //TODO - Implementar mkdir2
    return -1;
}

int rmdir2 (char *pathname) {
    //TODO - Implementar rmdir2
    return -1;
}

int chdir2 (char *pathname) {
    //TODO - Implementar chdir2
    return -1;
}

int getcwd2 (char *pathname, int size) {
    //TODO - Implementar getcwd2
    return -1;
}

DIR2 opendir2 (char *pathname) {
    //TODO - Implementar opendir2
    return -1;
}

int readdir2 (DIR2 handle, DIRENT2 *dentry) {
    //TODO - Implementar readdir2
    return -1;
}

int closedir2 (DIR2 handle) {
    //TODO - Implementar closedir2
    return -2;
}
