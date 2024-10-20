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
    usuario->moedas = 1000; // TESTE
    usuario->atq_basico = atq_inicial;
    usuario->atq_especial = (Habilidade){-1, "", 0, 0}; // inicia como valor padrao
    usuario->vida = 100; //exemplo
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        usuario->consumiveis[i] = (Item){-1, "", 0, 0, 0}; // inicia como valor padrao
    }
}

int login(Usuario *array_usuarios, int qnt_usuarios, Usuario **usuario_logado) {
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
        if (idx_usuario == FALHA) {
            print_erro("Credenciais incorretas. Insira novamente.\n");
            delay(1000);
        }        
    } while (idx_usuario == FALHA);

    *usuario_logado = &array_usuarios[idx_usuario];

    limpa_tela();
    print_sucesso("Login realizado com sucesso!\n");
    delay(1000);

    return OK;
}

int registro(Usuario *array_usuarios, int *qnt_usuarios, Habilidade atq_inicial) {
    Usuario novo_usuario;
    char entrada_login[TAM_LOGIN + 1], entrada_senha[TAM_SENHA + 1], entrada_nickname[TAM_NICK + 1]; //testar se o limite esta correto
    int auth;

    limpa_tela();

    if (*qnt_usuarios >= MAX_USUARIOS) {
        print_erro("Quantidade limite de usuarios criados atingida. Cancelando operacao...\n");
        return FALHA;
    }

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

    zerar_usuario(&novo_usuario, atq_inicial);
    strcpy(novo_usuario.nome_usuario, entrada_login);
    strcpy(novo_usuario.senha, entrada_senha);
    strcpy(novo_usuario.nickname, entrada_nickname);

    array_usuarios[*qnt_usuarios] = novo_usuario;
    (*qnt_usuarios)++;

    if (salvar_arquivo_bin("dados-usuarios.bin", array_usuarios, sizeof(Usuario), *qnt_usuarios) == FALHA) {
        return FALHA;
    }
    
    limpa_tela();
    print_sucesso("Usuario criado com sucesso!\n");

    voltar_menu();
    return OK;
}