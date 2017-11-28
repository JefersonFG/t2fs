/**
 * Implementação do gerenciador de sistema de arquivos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/fsmanager.h"

static struct t2fs_manager fs_manager;

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

int init_manager() {
    if (isInitialized == MANAGER_NOT_INITIALIZED) {
        unsigned char *buffer = (unsigned char *) malloc(SECTOR_SIZE);

        if (read_sector(0, buffer) != 0) {
            perror("Erro na leitura do superbloco");
            free(buffer);
            return -1;
        }

        memcpy(&fs_manager.superbloco, buffer, SECTOR_SIZE);

        if (init_fat() < 0)
            return -1;

        isInitialized = MANAGER_INITIALIZED;

        free(buffer);

        return 0;
    }

    return 1;
}

int init_fat() {
    fs_manager.fat.num_setores = fs_manager.superbloco.DataSectorStart - fs_manager.superbloco.pFATSectorStart;
    fs_manager.fat.num_clusters = fs_manager.fat.num_setores / fs_manager.superbloco.SectorsPerCluster;
    fs_manager.fat.data = malloc(fs_manager.fat.num_setores * SECTOR_SIZE);

    if (read_fat() < 0)
        return -1;

    return 0;
}

int read_fat() {
    unsigned char *buffer = (unsigned char *) malloc(SECTOR_SIZE);
    unsigned int i;

    unsigned int fat_begin = fs_manager.superbloco.pFATSectorStart;
    unsigned int fat_end = fs_manager.superbloco.DataSectorStart;

    for (i = fat_begin; i < fat_end; i++) {
        if (read_sector(i, buffer) != 0) {
            perror("Erro na leitura da fat");
            free(buffer);
            return -1;
        }

        memcpy(fs_manager.fat.data + (i - fat_begin) * SECTOR_SIZE, buffer, SECTOR_SIZE);
    }

    free(buffer);
    return 0;
}

int write_fat() {
    unsigned char *buffer = (unsigned char *) malloc(SECTOR_SIZE);
    unsigned int i;

    unsigned int fat_begin = fs_manager.superbloco.pFATSectorStart;
    unsigned int fat_end = fs_manager.superbloco.pFATSectorStart + fs_manager.fat.num_setores;

    for (i = fat_begin; i < fat_end; i++) {
        memcpy(buffer, fs_manager.fat.data + (i - fat_begin) * SECTOR_SIZE, SECTOR_SIZE);

        if (write_sector(i, buffer) != 0) {
            perror("Erro na escrita da fat");
            free(buffer);
            return -1;
        }
    }

    free(buffer);
    return 0;
}
