/**
 * Implementação do gerenciador de sistema de arquivos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/fsmanager.h"

unsigned int directory_max_entries;

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

/**
 * Inicializa o diretório de root lendo-o do disco ou criando-o e o escrevendo no disco.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int init_root_directory();

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

        /// Do espaço do cluster reduzimos os ponteiros para o diretório atual e diretório pai
        directory_max_entries = SECTOR_SIZE * fs_manager.superbloco.SectorsPerCluster / RECORD_SIZE - 2;

        if (init_root_directory())
            return -1;

        isInitialized = MANAGER_INITIALIZED;

        free(buffer);

        puts("Teste conteúdo root:\n");
        puts(fs_manager.diretorio_atual[0].name);
        puts(fs_manager.diretorio_atual[1].name);

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

int init_root_directory() {
    unsigned char *buffer = (unsigned char *) malloc(SECTOR_SIZE);

    unsigned int root_sector = fs_manager.superbloco.RootDirCluster;

    // TODO Corrigir essa compatação, FAT_EOF é para clusters e data nesse momento guarda setores
    if (fs_manager.fat.data[root_sector] != FAT_EOF) {
        puts("Criando root\n");

        fs_manager.diretorio_atual = (struct t2fs_record *) malloc(sizeof(struct t2fs_record)*directory_max_entries);
        struct t2fs_record *dot = (struct t2fs_record *) malloc(sizeof(struct t2fs_record));
        struct t2fs_record *dotdot = (struct t2fs_record *) malloc(sizeof(struct t2fs_record));

        dot->TypeVal = TYPEVAL_DIRETORIO;
        strcpy(dot->name, ".");
        dot->bytesFileSize = 0;
        dot->firstCluster = root_sector;

        *dotdot = *dot;
        strcpy(dotdot->name, "..");

        fs_manager.diretorio_atual[0] = *dot;
        fs_manager.diretorio_atual[1] = *dotdot;

        fs_manager.fat.data[root_sector] = FAT_EOF;

        root_sector += fs_manager.superbloco.DataSectorStart;

        // TODO Corrigir problema da fat e testar, só depois executar esse código (altera a base de dados)
        /*
        memcpy(buffer, fs_manager.diretorio_atual, SECTOR_SIZE);

        if (write_sector(root_sector, buffer) != 0) {
            perror("Erro na escrita do diretório raíz");
            free(buffer);
            return -1;
        }

        free(buffer);

        if (write_fat() < 0) {
            perror("Erro na escrita da fat");
            return -1;
        }
        */
    } else {
        puts("Lendo root");

        root_sector += fs_manager.superbloco.DataSectorStart;

        if (read_sector(root_sector, buffer) != 0) {
            perror("Erro na leitura do diretório raíz");
            free(buffer);
            return -1;
        }

        memcpy(fs_manager.diretorio_atual, buffer, SECTOR_SIZE);

        free(buffer);
    }

    return 0;
}
