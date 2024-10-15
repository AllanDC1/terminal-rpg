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

void verificar_buffer(char *entrada) {
    if (entrada[strlen(entrada) -1] != '\n') {
        limpar_buffer();
    } else {
        entrada[strcspn(entrada, "\n")] = 0;    
    }
}

void print_erro(char *texto) {
    printf("\033[0;31m%s\033[0m", texto);
}

void print_sucesso(char *texto) {
    printf("\033[1;32m%s\033[0m", texto);
}

void voltar_menu() {
    printf("\n\033[0;32m Pressione ENTER para voltar ao menu. \033[0m");
    getchar();
}

int iniciar_usuarios(Usuario **array_usuarios, int *qnt_usuarios) {    
    *qnt_usuarios = ler_arquivo_bin("dados-usuarios.bin", (void **)array_usuarios, sizeof(Usuario));
    if (*qnt_usuarios == FALHA) {
        print_erro("Falha ao carregar dados dos usuarios.\n");
        return FALHA;
    }
    return OK;
}

void limpa_tela () {
    #ifdef _WIN32
    system("cls"); // Para Windows
    #else
    system("clear"); // Para Unix/Linux (Replit)
    #endif
}
