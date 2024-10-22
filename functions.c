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

int iniciar_usuarios(Usuario *array_usuarios, int *qnt_usuarios) {    
    *qnt_usuarios = ler_arquivo_bin("dados-usuarios.bin", array_usuarios, sizeof(Usuario));
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

int iniciar_sistema(Usuario *array_usuarios, int *qnt_usuarios, Habilidade *array_habilidades, Dungeon *array_dungeons) {
    limpa_tela();

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

    if (ler_arq_dungeons(array_dungeons) == FALHA) {
        print_erro("Erro ao ler arquivo de dungeons.\n");
        return FALHA;        
    }

    return OK;
}

void encerrar_sistema(Usuario *array_usuarios, int qnt_usuarios) {
    salvar_arquivo_bin("dados-usuarios.bin", array_usuarios, sizeof(Usuario), qnt_usuarios);
    limpa_tela();
    printf("Obrigado pela sua presenca!\nEncerrando o programa...\n");
}

void verificar_nivel(Usuario *usuario_logado, Habilidade *array_habilidades) {
    for (int i = 0; i < QNT_HABILIDADES; i++) {
        if (usuario_logado->nivel >= array_habilidades[i].requisito_xp) {
            if (strcmp(usuario_logado->atq_basico.nome, array_habilidades[i].nome) != 0 &&
                strcmp(usuario_logado->atq_especial.nome, array_habilidades[i].nome) != 0)
                {
                printf("Nova habilidade desbloqueada: %s!\n", array_habilidades[i].nome);
                if (array_habilidades[i].ID % 2 == 0) {
                    usuario_logado->atq_basico = array_habilidades[i];
                } else {
                    usuario_logado->atq_especial = array_habilidades[i];
                }
            }
        }
    }
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

int comparar_por_ID_decrescente(const void *a, const void *b) {
    Dungeon *dungeonA = (Dungeon *)a;
    Dungeon *dungeonB = (Dungeon *)b;
    return dungeonB->ID - dungeonA->ID; // decrescente, se quiser colocar crescente tem q só mudar
}