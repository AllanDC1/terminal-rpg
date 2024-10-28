#include "../terminalrpg.h"

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
    size_t len = strlen(entrada);

    if (len > 0 && entrada[len - 1] == '\n') {
        entrada[len - 1] = '\0';
    } else {
        limpar_buffer();
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

void enter_continuar() {
    printf("\n\033[1;32m Pressione ENTER para continuar. \033[0m");
    getchar();
}

int iniciar_usuarios(Usuario *array_usuarios, int *qnt_usuarios) {    
    *qnt_usuarios = ler_arquivo_bin("assets/dados-usuarios.bin", array_usuarios, sizeof(Usuario));
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

int iniciar_sistema(Usuario *array_usuarios, int *qnt_usuarios, Habilidade *array_habilidades) {
    limpa_tela();
    srand(time(NULL));

    if (iniciar_usuarios(array_usuarios, qnt_usuarios) == FALHA) {
        print_erro("Erro ao iniciar usuarios.\n");
        return FALHA;
    }    

    if (criar_arq_habilidades() == FALHA) {
        print_erro("Erro ao criar arquivo de habilidades.\n");
        return FALHA;
    }

    if (criar_arq_itens() == FALHA) {
        print_erro("Erro ao criar arquivo de itens.\n");
        return FALHA;
    }

    if (criar_arq_dungeons() == FALHA) {
        print_erro("Erro ao criar arquivo de dungeons.\n");
        return FALHA;
    }

    if (criar_arq_inimigos() == FALHA) {
        print_erro("Erro ao criar arquivo de inimigos.\n");
        return FALHA;
    }

    if (ler_arq_habilidades(array_habilidades) == FALHA) {
        print_erro("Erro ao ler arquivo de habilidades.\n");
        return FALHA;        
    }

    return OK;
}

void encerrar_sistema(Usuario *array_usuarios, int qnt_usuarios) {
    salvar_arquivo_bin("assets/dados-usuarios.bin", array_usuarios, sizeof(Usuario), qnt_usuarios);
    limpa_tela();
    printf("Obrigado pela sua presenca!\nEncerrando o programa...\n");
}

int confirmar_acao() {
    int confirmacao;

    do{
        printf("\nConfirme sua acao:\n");
        printf("1-Confirmar ou 2-Cancelar: ");
        if (scanf("%d", &confirmacao) != 1 || confirmacao != 1 && confirmacao != 2) {
            print_erro("Escolha invalida.\n");
            confirmacao = FALHA;
        }            
        limpar_buffer();
    }while (confirmacao == FALHA);

    if (confirmacao == 1) {
        return OK;
    } else {
        return FALHA;
    }
}