/**
 * Gerenciador do sistema de arquivos, mantém controle do superbloco, da fat,
 * dos arquivos e diretórios abertos.
 */

#ifndef T2FS_FSMANAGER_H
#define T2FS_FSMANAGER_H

#include "t2fs.h"

struct t2fs_fat {
    char *data;
    int num_clusters;
};

struct t2fs_manager {
    struct t2fs_superbloco superbloco;
    struct t2fs_fat fat;
};

/**
 * Inicializa o gerenciador do sistema de arquivos, garantindo que a inicialização
 * só execute uma vez mesmo se a função for chamada mais de uma vez.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int init_manager();

#endif //T2FS_FSMANAGER_H
