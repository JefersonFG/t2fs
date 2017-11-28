/**
 * Implementação das funções de manipulação de arquivos e diretórios.
 */

#include <fsmanager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_operations.h"

//TODO Adicionar funções de manipulação de arquivos e diretórios.
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

T_OpenFile getFile(DWORD fatherReg, char *name){
    T_OpenFile file;
    struct t2fs_record *record = findRecord(fatherReg, name, -1);
    if(record == NULL || record->TypeVal != TYPEVAL_REGULAR){
        file.valid = -1;
    }
    else {
        file.valid = 1;
        file.curPointer = 0;
//        file.numBlocks = record->blocksFileSize;
        file.byteSize = record->bytesFileSize;
        file.firstCluster = record->firstCluster;
//        file.fatherMFT = fatherReg;
        strcpy(file.name, record->name);
    }
    return file;
}
