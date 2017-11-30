/**
 * Gerenciador do sistema de arquivos, mantém controle do superbloco, da fat,
 * dos arquivos e diretórios abertos.
 */

#ifndef T2FS_FSMANAGER_H
#define T2FS_FSMANAGER_H

#include "t2fs.h"
#include "file_operations.h"

#define RECORD_SIZE 64

#define FAT_FREE_CLUSTER 0x00000000
#define FAT_INVALID 0x00000001
#define FAT_BAD_SECTOR 0xFFFFFFFE
#define FAT_EOF 0xFFFFFFFF

#define MAX_OPEN_FILES 10
#define MAX_OPEN_DIRS 10

struct t2fs_record root;

unsigned int directory_max_entries;

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
    unsigned char *sectors;
    unsigned int num_setores;
    unsigned int num_clusters;
};

struct {
    struct t2fs_superbloco superbloco;
    struct t2fs_fat fat;
    T_OpenFile openFiles[MAX_OPEN_FILES];
    T_OpenDir openDirectories[MAX_OPEN_DIRS];
    int numOpenFiles;
    int numOpenDirectories;
    struct t2fs_record *diretorio_atual;
    struct t2fs_record *entradas_diretorio_atual;
} fs_manager;

/**
 * Inicializa o gerenciador do sistema de arquivos, garantindo que a inicialização
 * só execute uma vez mesmo se a função for chamada mais de uma vez.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int init_manager();

/**
 * Busca e retorna um cluster livre na fat.
 * @return Se obteve sucesso retorna o índice do cluster livre, caso contrário retorna um valor negativo.
 */
int get_free_cluster();

/**
 * Escreve a fat no disco.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int write_fat();

/**
 * Lê os dados do cluster requisitado.
 * @param cluster Cluster a ser lido
 * @param data Ponteiro para área de memória a ser preenchida com os dados
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int readClusterData(int cluster, BYTE* data);

/**
 * Lê os dados de uma entrada da FAT.
 * @param cluster Primeiro cluster a ser lido
 * @param sizeInBytes Tamanho do arquivo em bytes
 * @param data Ponteiro para área de memória a ser preenchida com os dados
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int readEntry(int cluster, BYTE* data);

#endif //T2FS_FSMANAGER_H
