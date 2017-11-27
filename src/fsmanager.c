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

int init_fat();

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
    unsigned int numSetores = fs_manager.superbloco.DataSectorStart - fs_manager.superbloco.pFATSectorStart;
    unsigned int numClusters = numSetores / fs_manager.superbloco.SectorsPerCluster;

    fs_manager.fat.num_clusters = numClusters;
    fs_manager.fat.data = malloc(numSetores * SECTOR_SIZE);

    unsigned char *buffer = (unsigned char *) malloc(SECTOR_SIZE);
    unsigned int i;

    for (i = fs_manager.superbloco.pFATSectorStart; i < fs_manager.superbloco.DataSectorStart; i++) {
        if (read_sector(i, buffer) != 0) {
            perror("Erro na leitura da fat");
            free(buffer);
            return -1;
        }

        memcpy(fs_manager.fat.data + (i - fs_manager.superbloco.pFATSectorStart) * SECTOR_SIZE, buffer, SECTOR_SIZE);
    }

    free(buffer);

    return 0;
}
