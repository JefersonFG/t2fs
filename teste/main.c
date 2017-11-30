#include <stdio.h>
#include <fsmanager.h>

#define IDENTIFICATION_SIZE 110

int main() {
    char identification[IDENTIFICATION_SIZE];

    if (init_manager() < 0)
        puts("Erro na inicialização!\n");

    printf("Identificacao do grupo:\n");
    identify2(identification, IDENTIFICATION_SIZE);
    puts(identification);

//    if (mkdir2("testdir") < 0)
//        puts("Erro ao criar um novo diretório");

//    BYTE* data = malloc(65532);
//    readEntry(fs_manager.superbloco.RootDirCluster,data);
//    printf("teste");

    return 0;
}
