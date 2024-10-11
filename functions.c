#include "terminalrpg.h"

//Funções gerais do programa

void delay(int tempo_ms) {
    #ifdef _WIN32
    Sleep(tempo_ms);
    #else
    usleep(tempo_ms * 1000); // mili * 1000 -> micro
    #endif
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void print_erro(char *texto) {
    printf("\033[0;31m%s\033[0m", texto);
}

int iniciar_usuarios(Usuario **array_usuarios, int *qnt_usuarios) {    
    *qnt_usuarios = ler_arquivo("dados-usuarios.bin", (void **)&array_usuarios, sizeof(Usuario), true);
    if (qnt_usuarios == FALHA) {
        print_erro("Falha ao carregar dados dos usuarios.\n");
        return FALHA;
    }
    return OK;
}