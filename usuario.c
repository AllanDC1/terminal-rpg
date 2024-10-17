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
    limpa_tela();
    print_erro("Credenciais incorretas. Insira novamente.\n");
    delay(1000);
    return FALHA;
}

int validar_nome_usuario(char *entrada, Usuario *array_usuarios, int qnt_usuarios) {
    if (strlen(entrada) > TAM_LOGIN) {
        limpa_tela();
        print_erro("Nome de usuario muito longo, limite de 20 caracteres.\n");
        delay(1000);
        return FALHA;
    }
    for (int i = 0; i < qnt_usuarios; i++) {
        if (strcmp(entrada, array_usuarios[i].nome_usuario) == 0) {
            limpa_tela();
            print_erro("O nome de usuario inserido ja possui uma conta. Voltando ao menu...\n");
            delay(1000);
            return SAIDA;
        }
    }
    return OK;
}

int validar_senha(char *entrada) {
    if (strlen(entrada) > TAM_SENHA || strlen(entrada) < 6) {
        limpa_tela();
        print_erro("Tamanho de senha invalida, deve ter entre 6 e 15 caracteres.\n");
        delay(1000);
        return FALHA;
    }
    return OK;
}

int validar_nickname(char *entrada) {
    if (strlen(entrada) > TAM_NICK) {
        limpa_tela();
        print_erro("Nickname muito longo, limite de 30 caracteres.\n");
        delay(1000);
        return FALHA;
    }
    return OK;
}

void zerar_usuario(Usuario *usuario, Habilidade atq_inicial) {    
    usuario->nivel = 0.0;
    usuario->moedas = 0;
    usuario->atq_basico = atq_inicial;
    //usuario->atq_especial = NULL; // testar pra ver se precisara declarar o atq especial como vazio inicialmente
    usuario->vida = 100; //exemplo
}