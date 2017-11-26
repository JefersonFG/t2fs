/**
 * Implementação do gerenciador de sistema de arquivos.
 */

#include <stdio.h>
#include <string.h>

#include "../include/t2fs.h"
#include "../include/apidisk.h"
#include "../include/fsmanager.h"

static struct t2fs_manager fs_manager;

#define MANAGER_INITIALIZED 1
#define MANAGER_NOT_INITIALIZED 0

static int isInitialized = MANAGER_NOT_INITIALIZED;

int init_manager() {
    if (isInitialized == MANAGER_NOT_INITIALIZED) {
        unsigned char buffer[SECTOR_SIZE];

        if (read_sector(0, buffer) != 0) {
            perror("Erro na leitura do superbloco");
            return -1;
        }

        memcpy(&fs_manager.superbloco, buffer, SECTOR_SIZE);

        isInitialized = MANAGER_INITIALIZED;

        return 0;
    }

    return 1;
}
