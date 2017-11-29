/**
 * Gerenciador do sistema de arquivos, mantém controle do superbloco, da fat,
 * dos arquivos e diretórios abertos.
 */

#ifndef T2FS_FSMANAGER_H
#define T2FS_FSMANAGER_H

#include "t2fs.h"

#define RECORD_SIZE 64

#define FAT_FREE_CLUSTER 0x00000000
#define FAT_INVALID 0x00000001
#define FAT_BAD_SECTOR 0xFFFFFFFE
#define FAT_EOF 0xFFFFFFFF

struct t2fs_record root;

struct t2fs_fat {
    unsigned char *sectors;
    unsigned int num_setores;
    unsigned int num_clusters;
};

struct {
    struct t2fs_superbloco superbloco;
    struct t2fs_fat fat;
    struct t2fs_record *diretorio_atual;
    struct t2fs_record *entradas_diretorio_atual;
} fs_manager;

/**
 * Inicializa o gerenciador do sistema de arquivos, garantindo que a inicialização
 * só execute uma vez mesmo se a função for chamada mais de uma vez.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int init_manager();

#endif //T2FS_FSMANAGER_H
