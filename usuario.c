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
    
    return FALHA;
}

int validar_nome_usuario(char *entrada, Usuario *array_usuarios, int qnt_usuarios) {
    if (strlen(entrada) > TAM_LOGIN || strlen(entrada) < 4) {
        limpa_tela();
        print_erro("Nome de usuario invalido, deve possuir de 5 a 20 caracteres.\n");
        delay(2000);
        return FALHA;
    }
    for (int i = 0; i < qnt_usuarios; i++) {
        if (strcmp(entrada, array_usuarios[i].nome_usuario) == 0) {
            limpa_tela();
            print_erro("O nome de usuario inserido ja possui uma conta. Voltando ao menu...\n");
            delay(2000);
            return SAIDA;
        }
    }
    return OK;
}

int validar_senha(char *entrada) {
    if (strlen(entrada) > TAM_SENHA || strlen(entrada) < 6) {
        limpa_tela();
        print_erro("Tamanho de senha invalida, deve ter entre 6 e 15 caracteres.\n");
        delay(2000);
        return FALHA;
    }
    return OK;
}

int validar_nickname(char *entrada) {
    if (strlen(entrada) > TAM_NICK || strlen(entrada) < 4) {
        limpa_tela();
        print_erro("Nickname invalido, deve possuir de 5 a 30 caracteres.\n");
        delay(2000);
        return FALHA;
    }
    return OK;
}

void zerar_usuario(Usuario *usuario, Habilidade *habilidades) {    
    usuario->xp_usuario = 0;
    usuario->moedas = 100;
    usuario->atq_basico = habilidades[0];
    usuario->atq_especial = habilidades[1];
    usuario->vida = 100; //exemplo
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        usuario->consumiveis[i] = (Item){-1, "", 0, 0, 0}; // inicia como valor padrao
    }
}

int login(Usuario *array_usuarios, int qnt_usuarios, Usuario **usuario_logado) {
    char entrada_login[TAM_LOGIN + 2], entrada_senha[TAM_SENHA + 2]; //testar se o limite esta correto
    int idx_usuario;

    if (qnt_usuarios == 0) {
        print_erro("Nenhuma conta foi criada no sistema.\n");
        delay(1500);
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

int registro(Usuario *array_usuarios, int *qnt_usuarios, Habilidade *array_habilidades) {
    Usuario novo_usuario;
    char entrada_login[TAM_LOGIN + 2], entrada_senha[TAM_SENHA + 2], entrada_nickname[TAM_NICK + 2]; //testar se o limite esta correto
    int auth = FALHA;

    if (*qnt_usuarios >= MAX_USUARIOS) {
        print_erro("Quantidade limite de usuarios criados atingida. Cancelando operacao...\n");
        return FALHA;
    }

    do {
        limpa_tela();
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

    zerar_usuario(&novo_usuario, array_habilidades);
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

    enter_continuar();
    return OK;
}

int alterar_apelido(Usuario *usuario_logado) {
    char entrada_nickname[TAM_NICK + 2];
    do {
        printf("> Informe um novo nickname: ");
        fgets(entrada_nickname, sizeof(entrada_nickname), stdin);
        verificar_buffer(entrada_nickname);        
    } while (validar_nickname(entrada_nickname) == FALHA);
    
    if (confirmar_acao() == FALHA) {
        printf("Alteracao de apelido cancelada.\n");
        return FALHA;
    }

    strcpy(usuario_logado->nickname, entrada_nickname);
    return OK;
}

int excluir_conta(Usuario *array_usuarios, int *qnt_usuarios, Usuario *usuario_logado) {
    char entrada[TAM_LOGIN + 2];
    int idx_usuario;

    // achar idx do usuario no array
    for (int i = 0; i < *qnt_usuarios; i++) {
        if (strcmp(array_usuarios[i].nome_usuario, usuario_logado->nome_usuario) == 0) {
            idx_usuario = i;
            break;
        }
    }

    limpa_tela();
    printf("> Digite seu nome de usuario para confirmar a exclusao: ");
    fgets(entrada, sizeof(entrada), stdin);
    verificar_buffer(entrada);
    if (strcmp(entrada, usuario_logado->nome_usuario) == 0) {
        if (confirmar_acao() == FALHA) {
            printf("Exclusao de conta cancelada.\n");
            return FALHA;
        }
        for (int i = idx_usuario; i < *qnt_usuarios - 1; i++) {
            array_usuarios[i] = array_usuarios[i + 1];
        }
        (*qnt_usuarios)--;
        return SAIDA;        
    } else {
        print_erro("Nome de usuario incorreto. Exclusao cancelada.\n");
        voltar_menu();
        return OK;
    }    
}