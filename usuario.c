#include "terminalrpg.h"

//Funções relacionadas a criação, alteração e exclusão de usuários

int validar_usuario(char *entrada_login, char *entrada_senha, Usuario *array_usuarios, int qnt_usuarios) {
    for (int i = 0; i < qnt_usuarios; i++) {
        if (strcmp(entrada_login, array_usuarios[i].nome_usuario) == 0) {
            if (strcmp(entrada_senha, array_usuarios[i].senha) == 0) {
                return i;
            }
            else {
                return FALHA;
            }
        }
    }
    print_erro("Credenciais incorretas. Insira novamente.\n");
    return FALHA;
}