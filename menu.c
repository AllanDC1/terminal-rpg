#include "terminalrpg.h"

//Funções e exibições dos menus

int escolher_operacao(int qnt_operacoes) {
    int escolha;
    
    do {
        printf("> Escolha a operacao: ");
        if (scanf("%d", &escolha) != 1 || (escolha < 1 || escolha > qnt_operacoes) ) {
            print_erro("Operacao invalida.\n\n");
            escolha = FALHA;
        }
        limpar_buffer();
    }while (escolha == FALHA);

    return escolha;
}

int menu_inicial() {   
    printf("|---------------------------|\n");
    printf("|    1. Login               |\n");
    printf("|    2. Registrar-se        |\n");
    printf("|    3. Sair                |\n");
    printf("|---------------------------|\n");

    return escolher_operacao(3);
}

int login(Usuario *array_usuarios, int qnt_usuarios, Usuario *usuario_logado) {
    char entrada_login[TAM_LOGIN], entrada_senha[TAM_SENHA];    
    int idx_usuario;

    if (qnt_usuarios == 0) {
        print_erro("Nenhuma conta foi criada no sistema.\n");
        return FALHA;
    }

    do {
        printf("Realize seu login para entrar no jogo:\n");

        printf("> Nome de Usuario: ");
        fgets(entrada_login, sizeof(entrada_login), stdin);
        verificar_buffer(entrada_login);

        printf("> Senha: ");
        fgets(entrada_senha, sizeof(entrada_senha), stdin);
        verificar_buffer(entrada_senha);

        idx_usuario = validar_usuario(entrada_login, entrada_senha, array_usuarios, qnt_usuarios);

    } while (idx_usuario == FALHA);

    *usuario_logado = array_usuarios[idx_usuario];

    print_sucesso("Login realizado com sucesso!\n");
    return OK;
}