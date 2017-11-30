/**
 * Implementação do gerenciador de sistema de arquivos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fsmanager.h>

#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/fsmanager.h"

#define MANAGER_INITIALIZED 1
#define MANAGER_NOT_INITIALIZED 0

static int isInitialized = MANAGER_NOT_INITIALIZED;

/**
 * Inicializa a representação da fat na memória e a lê do disco.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int init_fat();

/**
 * Lê a fat do disco.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int read_fat();

/**
 * Escreve a fat no disco.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int write_fat();

/**
 * Inicializa o diretório de root lendo-o do disco ou criando-o e o escrevendo no disco.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int init_root_directory();

int init_manager() {
    if (isInitialized == MANAGER_NOT_INITIALIZED) {
        unsigned char *buffer = malloc(SECTOR_SIZE);

        if (read_sector(0, buffer) != 0) {
            perror("Erro na leitura do superbloco");
            free(buffer);
            return -1;
        }

        memcpy(&fs_manager.superbloco, buffer, SECTOR_SIZE);

        if (init_fat() < 0)
            return -1;

        /// Do espaço do cluster reduzimos os ponteiros para o diretório atual e diretório pai
        directory_max_entries = SECTOR_SIZE * fs_manager.superbloco.SectorsPerCluster / RECORD_SIZE - 2;

        if (init_root_directory())
            return -1;
        fs_manager.numOpenDirectories = 0;
        fs_manager.numOpenFiles = 0;

        for (int i = 0; i < MAX_OPEN_DIRS; ++i) {
            fs_manager.openDirectories[i].valid = -1;
        }

        for (int j = 0; j < MAX_OPEN_FILES; ++j) {
            fs_manager.openFiles[j].valid = -1;
        }

        isInitialized = MANAGER_INITIALIZED;

        free(buffer);

        puts("Teste conteúdo root:");
        puts(fs_manager.entradas_diretorio_atual[0].name);
        puts(fs_manager.entradas_diretorio_atual[1].name);

        return 0;
    }

    return 1;
}

int init_fat() {
    fs_manager.fat.num_setores = fs_manager.superbloco.DataSectorStart - fs_manager.superbloco.pFATSectorStart;
    fs_manager.fat.num_clusters = fs_manager.fat.num_setores / fs_manager.superbloco.SectorsPerCluster;
    fs_manager.fat.sectors = malloc(fs_manager.fat.num_setores * SECTOR_SIZE);

    if (read_fat() < 0)
        return -1;

    return 0;
}

int read_fat() {
    unsigned char *buffer = malloc(SECTOR_SIZE);
    unsigned int i;

    unsigned int fat_begin = fs_manager.superbloco.pFATSectorStart;
    unsigned int fat_end = fs_manager.superbloco.DataSectorStart;

    for (i = fat_begin; i < fat_end; i++) {
        if (read_sector(i, buffer) != 0) {
            perror("Erro na leitura da fat");
            free(buffer);
            return -1;
        }

        memcpy(fs_manager.fat.sectors + (i - fat_begin) * SECTOR_SIZE, buffer, SECTOR_SIZE);
    }

    free(buffer);
    return 0;
}

int write_fat() {
    unsigned char *buffer = malloc(SECTOR_SIZE);
    unsigned int i;

    unsigned int fat_begin = fs_manager.superbloco.pFATSectorStart;
    unsigned int fat_end = fs_manager.superbloco.pFATSectorStart + fs_manager.fat.num_setores;

    for (i = fat_begin; i < fat_end; i++) {
        memcpy(buffer, fs_manager.fat.sectors + (i - fat_begin) * SECTOR_SIZE, SECTOR_SIZE);

        if (write_sector(i, buffer) != 0) {
            perror("Erro na escrita da fat");
            free(buffer);
            return -1;
        }
    }

    free(buffer);
    return 0;
}

int init_root_directory() {
    long int *cluster = malloc(SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);
    unsigned char *buffer = malloc(SECTOR_SIZE);

    unsigned int root_sector = fs_manager.superbloco.RootDirCluster;

    memcpy(cluster, &fs_manager.fat.sectors[root_sector], SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

    fs_manager.entradas_diretorio_atual = calloc(fs_manager.superbloco.SectorsPerCluster, SECTOR_SIZE);

    if (*cluster != FAT_EOF) {
        puts("Criando root\n");

        root.TypeVal = TYPEVAL_DIRETORIO;
        strcpy(root.name, "/");
        root.bytesFileSize = 0;
        root.firstCluster = root_sector;

        struct t2fs_record *dot = malloc(sizeof(struct t2fs_record));
        struct t2fs_record *dotdot = malloc(sizeof(struct t2fs_record));

        fs_manager.diretorio_atual = &root;

        dot->TypeVal = TYPEVAL_DIRETORIO;
        strcpy(dot->name, ".");
        dot->bytesFileSize = 0;
        dot->firstCluster = root_sector;

        *dotdot = *dot;
        strcpy(dotdot->name, "..");

        fs_manager.entradas_diretorio_atual[0] = *dot;
        fs_manager.entradas_diretorio_atual[1] = *dotdot;

        *cluster = FAT_EOF;

        memcpy(&fs_manager.fat.sectors[root_sector], cluster, SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

        memcpy(cluster, fs_manager.entradas_diretorio_atual, SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

        unsigned int i, j;
        root_sector += fs_manager.superbloco.DataSectorStart;

        for (i = root_sector, j = 0; j < fs_manager.superbloco.SectorsPerCluster; i += SECTOR_SIZE, j++) {
            memcpy(buffer, cluster + SECTOR_SIZE * j, SECTOR_SIZE);

            if (write_sector(i, buffer) != 0) {
                perror("Erro na escrita do diretório raíz");
                free(cluster);
                free(buffer);
                return -1;
            }
        }

        if (write_fat() < 0) {
            perror("Erro na escrita da fat");
            free(cluster);
            free(buffer);
            return -1;
        }

    } else {
        unsigned int i, j;
        root_sector += fs_manager.superbloco.DataSectorStart;

        for (i = root_sector, j = 0; j < fs_manager.superbloco.SectorsPerCluster; i += SECTOR_SIZE, j++) {
            if (read_sector(i, buffer) != 0) {
                perror("Erro na leitura do diretório raíz");
                free(cluster);
                free(buffer);
                return -1;
            }

            memcpy(cluster + SECTOR_SIZE * j, buffer, SECTOR_SIZE);
        }

        memcpy(fs_manager.entradas_diretorio_atual, cluster, SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);
    }

    free(cluster);
    free(buffer);

    return 0;
}

int get_free_cluster() {
    long int *cluster = malloc(SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);
    unsigned int i;

    for (i = 0; i < fs_manager.fat.num_clusters; i += fs_manager.superbloco.SectorsPerCluster) {
        memcpy(cluster, &fs_manager.fat.sectors[i], SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

        if (*cluster == FAT_FREE_CLUSTER) {
            free(cluster);
            return i;
        }
    }

    /// Sem clusters livres
    perror("Não há mais clusters livres");
    free(cluster);
    return -1;
}

int readClusterData(int cluster, BYTE* data) {
    BYTE* buffer = malloc(SECTOR_SIZE);
    unsigned int i, j;
    DWORD sector = cluster * fs_manager.superbloco.SectorsPerCluster + fs_manager.superbloco.DataSectorStart;

    if (cluster < 2){
        perror("Acesso a cluster inválido.");
        return -1;
    }
    else{
        for (i = sector, j = 0; j < fs_manager.superbloco.SectorsPerCluster; i += SECTOR_SIZE, j++) {
            if (read_sector(i, buffer) != 0) {
                perror("Erro na leitura do setor.");
                free(data);
                free(buffer);
                return -1;
            }

            memcpy(data + SECTOR_SIZE * j, buffer, SECTOR_SIZE);
        }
    }

    free(buffer);

    return 0;
}

int readEntry(int cluster, BYTE* data) {
    BYTE* buffer = malloc(SECTOR_SIZE * fs_manager.superbloco.SectorsPerCluster);
    long int* fatPointer = malloc(sizeof(long int));
    int i = 0;

    do {
        memcpy(fatPointer,&fs_manager.fat.sectors[cluster], sizeof(DWORD));

        if (*fatPointer == FAT_BAD_SECTOR || *fatPointer == FAT_INVALID){
            perror("Entrada FAT inválida.");
            free(buffer);
            free(fatPointer);
            return -1;
        }
        if (readClusterData(cluster, buffer)){
            printf("Erro ao ler o cluster %d\n",cluster);
            free(buffer);
            free(fatPointer);
            return -1;
        }

        memcpy(data + (i * (SECTOR_SIZE * fs_manager.superbloco.SectorsPerCluster)), buffer, SECTOR_SIZE * fs_manager.superbloco.SectorsPerCluster);

        cluster = *fatPointer;
        ++i;
    } while(*fatPointer != FAT_EOF);

    free(buffer);
    free(fatPointer);

    return 0;
}
