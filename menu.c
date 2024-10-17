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
        if (idx_usuario == FALHA) {
            print_erro("Credenciais incorretas. Insira novamente.\n");
            delay(1000);
        }        
    } while (idx_usuario == FALHA);

    *usuario_logado = array_usuarios[idx_usuario];

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

// funcoes para o menu principal do personagem provisorio

int menu_principal() {   
    printf("|---------------------------|\n");
    printf("|    1. Jogar               |\n");
    printf("|    2. Inventario          |\n");
    printf("|    3. Loja                |\n");
    printf("|    4. Modificar Conta     |\n");
    printf("|    5. Sair                |\n");
    printf("|---------------------------|\n");

    return escolher_operacao(5);
}

// FUNÇÕES PARA A LOJA

int menu_itens_compraveis(Usuario* usuario_logado) {
    Item array_itens[QNT_CONSUMIVEIS];
    int id_desejado;
    
    if (ler_arq_itens(array_itens) == FALHA){
        print_erro("Erro ao ler os itens.\n");
        return FALHA;
    }

    limpa_tela();

    // Cabeçalho da tabela
    printf("BEM-VINDO A LOJA!\n\n");
    printf("---------------------------------------------------------------\n");
    printf("%-5s %-25s %-10s %-10s %-10s\n", "ID", "Nome", "Vida (%)", "Forca (%)", "Preco");
    printf("---------------------------------------------------------------\n");
    // Printa lista de itens
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        printf("%-5d %-25s %-10d %-10d %-10d\n", 
            array_itens[i].ID,
            array_itens[i].nome, 
            array_itens[i].vida_recuperada, 
            array_itens[i].dano_aumentado, 
            array_itens[i].preco
        );
    }

    // Pede o ID do item
    printf("\nDigite o 0 para sair ou ID do item desejado: ");
    while (scanf("%d", &id_desejado) != 1 || id_desejado < 0 || id_desejado > QNT_CONSUMIVEIS - 1) {
        print_erro("ID invalido. Insira novamente.\n");
        limpar_buffer();
    }
    limpar_buffer();

    if (id_desejado == 0){return OK;}

    limpa_tela();
    printf("\nVocê escolheu a poção:\n");
    printf("ID: %d\n", array_itens[id_desejado].ID);
    printf("Nome: %s\n", array_itens[id_desejado].nome);
    printf("Vida Recuperada: %d\n", array_itens[id_desejado].vida_recuperada);
    printf("Dano Aumentado: %d\n", array_itens[id_desejado].dano_aumentado);
    printf("Preco: %d\n", array_itens[id_desejado].preco);

    if (array_itens[id_desejado].preco > usuario_logado->moedas) {
        limpa_tela();
        print_erro("Voce nao possui moedas suficientes para esse item.\n");
        delay(1000);
        return FALHA; // cancela compra
    }

    // Pedir confirmacao talvez

    usuario_logado->consumiveis[array_itens[id_desejado].ID] = array_itens[id_desejado];
    usuario_logado->moedas -= array_itens[id_desejado].preco;

    print_sucesso("Compra bem sucedida!\n");
    return OK;
}

// Funcoes de exibição de inventario

 