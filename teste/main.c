#include <stdio.h>
#include "../include/t2fs.h"
#include "../include/fsmanager.h"

#define IDENTIFICATION_SIZE 110

int main() {
    char identification[IDENTIFICATION_SIZE];

    init_manager();

    printf("Identificacao do grupo:\n");
    identify2(identification, IDENTIFICATION_SIZE);
    puts(identification);

    return 0;
}
