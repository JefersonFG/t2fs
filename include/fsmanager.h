/**
 * Gerenciador do sistema de arquivos, mantém controle do superbloco, da fat,
 * dos arquivos e diretórios abertos.
 */

#ifndef T2FS_FSMANAGER_H
#define T2FS_FSMANAGER_H

/**
 * Inicializa o gerenciador do sistema de arquivos.
 * @return Se obteve sucesso retorna 0, caso contrário retorna um valor negativo.
 */
int init_manager();

#endif //T2FS_FSMANAGER_H
