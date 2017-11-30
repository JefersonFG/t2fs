/**
 * Implementação da interface do t2fs.
 */

#include <stdio.h>
#include <string.h>
#include <file_operations.h>

#define IDENTIFICACAO_GRUPO "Jeferson Ferreira Guimaraes - 262522\n" \
"Luis Miguel Santos Batista - 265037\nVinicius Chagas Soares - 262510\n"

int identify2 (char *name, int size) {
    int tamanho_identificacao = (int) strlen(IDENTIFICACAO_GRUPO);

    if (size < tamanho_identificacao)
        return -1;

    strcpy(name, IDENTIFICACAO_GRUPO);

    return 0;
}

FILE2 create2 (char *filename) {
    //TODO - Implementar create2
    return -1;
}

int delete2 (char *filename) {
    //TODO - Implementar delete2
    return -1;
}

FILE2 open2 (char *filename) {
    //TODO - Implementar open2
    char* name;
    char* parentDir;
    FILE2 handle;
    init_manager();

    name = getFileName(filename);
    if (name == NULL) return -1;

    handle = getNewHandle(TYPEVAL_REGULAR);
    if (handle == NULL) return -1;

//    fs_manager.openFiles[handle] = openFile(parentDir, name);
    return handle;
}

int close2 (FILE2 handle) {
    //TODO - Implementar close2
    return -1;
}

int read2 (FILE2 handle, char *buffer, int size) {
    //TODO - Implementar read2
    return -1;
}

int write2 (FILE2 handle, char *buffer, int size) {
    //TODO - Implementar write2
    return -1;
}

int truncate2 (FILE2 handle) {
    //TODO - Implementar truncate2
    return -1;
}

int seek2 (FILE2 handle, unsigned int offset) {
    //TODO - Implementar seek2
    return -1;
}

int mkdir2 (char *pathname) {
    if (init_manager() < 0)
        return -1;

    char path_before_new_dir[strlen(pathname)];
    char *last_separator;
    char *directory_name;
    char separator = '/';

    struct t2fs_record father_list;

    strcpy(path_before_new_dir, pathname);
    last_separator = strrchr(path_before_new_dir, separator);

    // TODO Verificar se o arquivo existe antes de criar!

    if (last_separator != NULL)
        directory_name = last_separator + 1;
    else
        directory_name = pathname;


    if (last_separator == NULL) {
        /// Insere novo subdiretório no diretório atual
        if (create_dir(directory_name, fs_manager.diretorio_atual, fs_manager.entradas_diretorio_atual) < 0)
            return -1;
    } else if (strcmp(pathname, last_separator) == 0) {
        /// Insere novo diretório no root
        if (get_directory_entries(&root, &father_list) < 0)
            return -1;
        if (create_dir(directory_name, &root, &father_list) < 0)
            return -1;
    } else {
        *last_separator = '\0';

        if (get_father_dir(path_before_new_dir, &father_list) < 0) {
            perror("Caminho inválido");
            return -1;
        }

        // TODO Criar e inserir diretório na lista do registro pai
    }

    return -1;
}

int rmdir2 (char *pathname) {
    //TODO - Implementar rmdir2
    return -1;
}

int chdir2 (char *pathname) {
    //TODO - Implementar chdir2
    return -1;
}

int getcwd2 (char *pathname, int size) {
    //TODO - Implementar getcwd2
    return -1;
}

DIR2 opendir2 (char *pathname) {
    //TODO - Implementar opendir2
    return -1;
}

int readdir2 (DIR2 handle, DIRENT2 *dentry) {
    //TODO - Implementar readdir2
    return -1;
}

int closedir2 (DIR2 handle) {
    //TODO - Implementar closedir2
    return -2;
}
