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
    char entrada_login[TAM_LOGIN + 1], entrada_senha[TAM_SENHA + 1]; //testar se o limite esta correto
    int idx_usuario;

    if (qnt_usuarios == 0) {
        print_erro("Nenhuma conta foi criada no sistema.\n");
        delay(1000);
        return FALHA;
    }

    do {
        limpa_tela();
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

int registro(Usuario *array_usuarios, int *qnt_usuarios) {
    Usuario novo_usuario;
    char entrada_login[TAM_LOGIN + 1], entrada_senha[TAM_SENHA + 1], entrada_nickname[TAM_NICK + 1]; //testar se o limite esta correto
    int auth;

    limpa_tela();

    do {
        printf("Registre sua conta:\n");

        printf("> Informe nome de usuario (Usado para login): ");
        fgets(entrada_login, sizeof(entrada_login), stdin);
        verificar_buffer(entrada_login);
        auth = validar_nome_usuario(entrada_login, array_usuarios, *qnt_usuarios);

        if (auth == SAIDA) {
            return FALHA; // volta pro menu
        }        
    } while (auth == FALHA);

    do {
        printf("> Informe uma senha (6-15 caracteres): ");
        fgets(entrada_senha, sizeof(entrada_senha), stdin);
        verificar_buffer(entrada_senha);        
    } while (validar_senha(entrada_senha) == FALHA);

    do {
        printf("> Informe um nickname (Nome mostrado em jogo): ");
        fgets(entrada_nickname, sizeof(entrada_nickname), stdin);
        verificar_buffer(entrada_nickname);        
    } while (validar_nickname(entrada_nickname) == FALHA);

    zerar_usuario(&novo_usuario);
    strcpy(novo_usuario.nome_usuario, entrada_login);
    strcpy(novo_usuario.senha, entrada_senha);
    strcpy(novo_usuario.nickname, entrada_nickname);

    array_usuarios[*qnt_usuarios] = novo_usuario;
    (*qnt_usuarios)++;

    if (salvar_arquivo("dados-usuarios.bin", array_usuarios, sizeof(Usuario), *qnt_usuarios, true) == FALHA) {
        return FALHA;
    }

    print_sucesso("Usuario criado com sucesso!\n");

    voltar_menu();
    return OK;
}

// funcoes para o menu principal do personagem provisorio

int menu_principal() {   
    printf("|---------------------------|\n");
    printf("|    1. batalhar            |\n");
    printf("|    2. inventario          |\n");
    printf("|    3. loja                |\n");
    printf("|    4. Sair                |\n");
    printf("|---------------------------|\n");

    return escolher_operacao(4);
}