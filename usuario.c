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

int validar_nome_usuario(char *entrada, Usuario *array_usuarios, int qnt_usuarios) {
    if (strlen(entrada) > TAM_LOGIN) {
        print_erro("Nome de usuario muito longo, limite de 20 caracteres.\n");
        return FALHA;
    }
    for (int i = 0; i < qnt_usuarios; i++) {
        if (strcmp(entrada, array_usuarios[i].nome_usuario) == 0) {
            print_erro("O nome de usuario inserido ja possui uma conta. Voltando ao menu...\n");
            return SAIDA;
        }
    }
    return OK;
}

int validar_senha(char *entrada) {
    if (strlen(entrada) > TAM_SENHA || strlen(entrada) < 6) {
        print_erro("Tamanho de senha invalida, deve ter entre 6 e 15 caracteres.\n");
        return FALHA;
    }
    return OK;
}

int validar_nickname(char *entrada) {
    if (strlen(entrada) > TAM_NICK) {
        print_erro("Nickname muito longo, limite de 30 caracteres.\n");
        return FALHA;
    }
    return OK;
}

void zerar_usuario(Usuario *usuario) {
    usuario->nivel = 0.0;
    //usuario->inventario[0] = habilidades[0]; atribuir a habilidade inicial
   // usuario->status.dano = inventario[0].dano; relacionar a habilidade
   usuario->status.vida = 100; //exemplo
}