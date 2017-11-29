/**
 * Gerenciador do sistema de arquivos, mantém controle do superbloco, da fat,
 * dos arquivos e diretórios abertos.
 */

#ifndef T2FS_FSMANAGER_H
#define T2FS_FSMANAGER_H

#include "t2fs.h"
#include "file_operations.h"

typedef struct {
    int valid;
    DWORD firstCluster;
    DWORD curPointer;
    DWORD numBlocks;
    DWORD byteSize;
    char name[MAX_FILE_NAME_SIZE];
} T_OpenFile;

typedef struct {
    int valid;
    DWORD firstCluster;
    DWORD curEntry;
    DWORD numBlocks;
    DWORD byteSize;
    char name[MAX_FILE_NAME_SIZE];
} T_OpenDir;

struct t2fs_fat {
    char *data;
    unsigned int num_setores;
    unsigned int num_clusters;
};

struct t2fs_manager {
    struct t2fs_superbloco superbloco;
    struct t2fs_fat fat;
    T_OpenFile openFiles[MAX_OPEN_FILES];
    T_OpenDir openDirectories[MAX_OPEN_DIRS];
    int numOpenFiles;
    int numOpenDirectories;
};

static struct t2fs_manager fs_manager;


/**
 * Inicializa o gerenciador do sistema de arquivos, garantindo que a inicialização
 * só execute uma vez mesmo se a função for chamada mais de uma vez.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int init_manager();

/**
 * Lê os dados do cluster requisitado.
 * @param cluster Cluster a ser lido
 * @param data Ponteiro para área de memória a ser preenchida com os dados
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int readClusterData(DWORD cluster, BYTE* data);

/**
 * Lê os dados de uma entrada da FAT.
 * @param cluster Primeiro cluster a ser lido
 * @param sizeInBytes Tamanho do arquivo em bytes
 * @param data Ponteiro para área de memória a ser preenchida com os dados
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int readFATEntry(DWORD cluster, DWORD sizeInBytes, BYTE* data);

#endif //T2FS_FSMANAGER_H
