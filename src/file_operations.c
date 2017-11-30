/**
 * Implementação das funções de manipulação de arquivos e diretórios.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/file_operations.h"
#include "../include/apidisk.h"

int get_directory_entries(struct t2fs_record *diretorio, struct t2fs_record *entradas_diretorio) {
    if (diretorio == fs_manager.diretorio_atual) {
        entradas_diretorio = fs_manager.entradas_diretorio_atual;
    } else {
        unsigned int i, j;
        unsigned int sector = diretorio->firstCluster + fs_manager.superbloco.DataSectorStart;

        unsigned char *buffer = malloc(SECTOR_SIZE);
        long int *cluster = malloc(SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

        for (i = sector, j = 0; j < fs_manager.superbloco.SectorsPerCluster; i += SECTOR_SIZE, j++) {
            if (read_sector(i, buffer) != 0) {
                perror("Erro na leitura do diretório raíz");
                free(cluster);
                free(buffer);
                return -1;
            }

            memcpy(cluster + SECTOR_SIZE * j, buffer, SECTOR_SIZE);
        }

        memcpy(entradas_diretorio, cluster, SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

        free(cluster);
        free(buffer);
    }

    return 0;
}

int get_father_dir(char *pathname, struct t2fs_record* entradas_diretorio_pai) {
    // TODO Implementar get_father_dir
    return -1;
}

int create_dir(char *nome_diretorio, struct t2fs_record *diretorio_pai, struct t2fs_record *entradas_diretorio_pai) {
    struct t2fs_record novo_diretorio;
    struct t2fs_record *entradas_novo_diretorio = calloc(fs_manager.superbloco.SectorsPerCluster, SECTOR_SIZE);
    int free_cluster;

    free_cluster = get_free_cluster();

    if (free_cluster < 0) {
        free(entradas_novo_diretorio);
        return -1;
    }

    novo_diretorio.TypeVal = TYPEVAL_DIRETORIO;
    strcpy(novo_diretorio.name, nome_diretorio);
    novo_diretorio.bytesFileSize = 0;
    novo_diretorio.firstCluster = (unsigned int) free_cluster;

    // TODO Criar lista de entradas do novo diretório e incluí-lo na lista do diretório pai

    struct t2fs_record *dot = malloc(sizeof(struct t2fs_record));
    struct t2fs_record *dotdot = malloc(sizeof(struct t2fs_record));

    *dot = novo_diretorio;
    strcpy(dot->name, ".");

    *dotdot = *dot;
    strcpy(dotdot->name, "..");
    dotdot->firstCluster = diretorio_pai->firstCluster;

    entradas_novo_diretorio[0] = *dot;
    entradas_novo_diretorio[1] = *dotdot;

    unsigned char *buffer = malloc(SECTOR_SIZE);
    long int *cluster = malloc(SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);
    memcpy(cluster, &fs_manager.fat.sectors[free_cluster], SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

    *cluster = FAT_EOF;

    memcpy(&fs_manager.fat.sectors[free_cluster], cluster, SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

    memcpy(cluster, entradas_novo_diretorio, SECTOR_SIZE*fs_manager.superbloco.SectorsPerCluster);

    unsigned int i, j;
    free_cluster += fs_manager.superbloco.DataSectorStart;

    for (i = (unsigned int) free_cluster, j = 0; j < fs_manager.superbloco.SectorsPerCluster; i += SECTOR_SIZE, j++) {
        memcpy(buffer, cluster + SECTOR_SIZE * j, SECTOR_SIZE);

        if (write_sector(i, buffer) != 0) {
            perror("Erro na escrita do novo diretório");
            free(entradas_novo_diretorio);
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

    // TODO Adicionar na lista de entradas do diretório pai
}

FILE2 getNewHandle(int type) {
    FILE2 handle;
    switch(type){
        case TYPEVAL_REGULAR:
            for(handle = 0; handle < MAX_OPEN_FILES; ++handle){
                if (fs_manager.openFiles[handle].valid == -1) return handle;
            }
            break;
        case TYPEVAL_DIRETORIO:
            for (handle = 0; handle < MAX_OPEN_DIRS; ++handle) {
                if (fs_manager.openDirectories[handle].valid == -1) return handle;
            }
            break;
        default:
            return -1;
    }
    printf("Numero de arquivos abertos simultaneamente excedido.");
    return -1;
}

char *getFileName(char *filename) {
    char *pointer;
    pointer = strrchr(filename, '/');
    return pointer ? pointer + 1 : NULL;
}

//TODO: IMPLEMENTAR FINDRECORD
struct t2fs_record* findRecord(DWORD cluster, char* name, DWORD entry);

//T_OpenFile getFile(DWORD cluster, char *name){
//    T_OpenFile file;
//    struct t2fs_record *record = findRecord(cluster, name, -1);
//    if(record == NULL || record->TypeVal != TYPEVAL_REGULAR){
//        file.valid = -1;
//    }
//    else {
//        file.valid = 1;
//        file.curPointer = 0;
//        file.byteSize = record->bytesFileSize;
//        file.firstCluster = record->firstCluster;
//        strcpy(file.name, record->name);
//    }
//    return file;
//}
