#include "terminalrpg.h"

//Funções relacionadas ao funcionamento/mecânicas do jogo

int jogar(Usuario *usuario_logado) {
    PlayerBatalha jogador = iniciar_jogador(usuario_logado);
    Dungeon dungeons[QNT_DUNGEONS];
    
    if (ler_arq_dungeons(dungeons) == FALHA) {
        print_erro("Erro ao ler arquivo de dungeons.\n");
        return FALHA;        
    }

    int id_dungeon_escohlida = selecao_dungeon(dungeons);
    int idx_dungeon = id_dungeon_escohlida - 1;

    Inimigo lista_inimigos[4];
    if (gerar_inimigos(lista_inimigos, id_dungeon_escohlida, usuario_logado->vida) == FALHA) {
        print_erro("Erro ao gerar inimigos da dungeon.\n");
        return FALHA;
    }

    do {
        exibir_combate(jogador, lista_inimigos, dungeons[idx_dungeon].nome, 1);
        // Turno do jogador
        switch (menu_combate()) {
            case 1:
                atacar(&jogador, lista_inimigos);
                break;
            case 2:
                //usar item
                usar_itens(usuario_logado, &jogador);
                break;
            case 3:
                if (tentar_fuga() == OK) {
                    print_sucesso("Voce conseguiu escapar.\n");
                    delay(1000);
                    return SAIDA;
                } else {
                    print_erro("Voce falhou na fuga. Turno perdido.\n");
                    delay(1000);
                }
                break;
        }

    } while (verificar_fim_combate(jogador, lista_inimigos) == CONTINUAR);
    
    
    // na ultima camdada
    //exibir_combate_boss(jogador, lista_inimigos[3], dungeons[idx_dungeon].nome);
    
}

PlayerBatalha iniciar_jogador(Usuario *usuario_logado) {
    PlayerBatalha jogador = {
        .vida_base = usuario_logado->vida,
        .vida_atual = usuario_logado->vida,
        .atq_basico  = usuario_logado->atq_basico,
        .atq_especial = usuario_logado->atq_especial,
        .dano_multiplicado = 1.0
    };
    strcpy(jogador.nick, usuario_logado->nickname);    

    return jogador;
}

int selecao_dungeon(Dungeon *array_dungeons) {
    limpa_tela();
    printf("Selecione a dungeon que deseja jogar:\n");
    exibir_dungeons(array_dungeons);
    
    return escolher_operacao(QNT_DUNGEONS, "a dungeon");
}

int gerar_inimigos(Inimigo *array_inimigos, int id_dungeon_escolhida, int vida_usuario) {
    Inimigo todos_inimigos[QNT_INIMIGOS];
    
    if (ler_arq_inimigos(todos_inimigos) == FALHA) {
        print_erro("Erro ao ler arquivo de inimigos.\n");
        return FALHA;    
    }

    float porcentagem_vida[3] = {0.15, 0.15, 0.15}; // da pra mudar a porcentagem da vida dos monstros !!
    
    for (int i = 0; i < QNT_INIMIGOS; i++) {
        if (todos_inimigos[i].id_dungeon == id_dungeon_escolhida) {
            // Gerar os 3 primeiros monstros encontrados para a dungeon
            for (int j = 0; j < 3; j++) {
                array_inimigos[j] = todos_inimigos[i + j];
                
                // Ajustar a vida conforme a porcentagem
                array_inimigos[j].vida_total = array_inimigos[j].vida_total + (vida_usuario * porcentagem_vida[j]);
                array_inimigos[j].vida_atual = array_inimigos[j].vida_total;
                
            }
            break;
        }
    }

    return OK;
}

void exibir_combate(PlayerBatalha jogador, Inimigo *inimigos, const char *nome_dungeon, int camada) {
    limpa_tela();
    printf("                      %s %d\n", nome_dungeon, camada);
    
    printf("--------------------------------------------------------------------\n");
    if (inimigos[0].vida_atual > 0) {
        printf("                              /    (%s) [Vida: %d/%d]\n", 
               inimigos[0].nome, inimigos[0].vida_atual, inimigos[0].vida_total);
    } else {
        printf("                              /    (%s) Derrotado\n", inimigos[0].nome);
    }

    printf("(%s) [Vida: %d/%d]    /    ", jogador.nick, jogador.vida_atual, jogador.vida_base);

    if (inimigos[1].vida_atual > 0) {
        printf("(%s) [Vida: %d/%d]\n", inimigos[1].nome, inimigos[1].vida_atual, inimigos[1].vida_total);
    } else {
        printf("(%s) Derrotado\n", inimigos[1].nome);
    }

    if (inimigos[2].vida_atual > 0) {
        printf("                            /    (%s) [Vida: %d/%d]\n", 
               inimigos[2].nome, inimigos[2].vida_atual, inimigos[2].vida_total);
    } else {
        printf("                            /    (%s) Derrotado\n", inimigos[2].nome);
    }
    printf("--------------------------------------------------------------------\n");
}

void exibir_combate_boss(PlayerBatalha jogador, Inimigo boss, const char *nome_dungeon) {
    limpa_tela();
    printf("                      %s\n", nome_dungeon);
    
    printf("--------------------------------------------------------------------\n");
    printf("                              /             \n");  // Linha vazia superior
    printf("(%s) [Vida: %d/%d]    /    (%s) [Vida: %d/%d]\n", 
            jogador.nick, jogador.vida_atual, jogador.vida_base,
            boss.nome, boss.vida_atual, boss.vida_total);
    printf("                            /             \n");  // Linha vazia inferior
    printf("--------------------------------------------------------------------\n");
}

int verificar_fim_combate(PlayerBatalha jogador, Inimigo *inimigos) {
    if (jogador.vida_atual <= 0) {
        return DERROTA;
    } 
    if (inimigos[0].vida_atual <=0 && inimigos[1].vida_atual <=0 && inimigos[2].vida_atual <=0) {
        return VITORIA;
    }
    
    return CONTINUAR;
}

int menu_combate() {
    printf("|--------------------|\n");
    printf("|   1. Atacar        |\n");
    printf("|   2. Usar Item     |\n");
    printf("|   3. Fugir         |\n");
    printf("|--------------------|\n");

    return escolher_operacao(3, "a sua acao");
}

int tentar_fuga() {
    srand(time(NULL)); 
    int tentativa = rand() % 100; // 0 a 99

    if (tentativa < 90) {
        return OK;
    } else {
        return FALHA; 
    }
}

int escolha_ataque(PlayerBatalha* jogador) {
    limpa_tela();
    printf("|--------------------------------------------------|\n");
    printf("|                                                  |\n");
    printf("|  1.%-20s  2.%-20s  |\n", jogador->atq_basico.nome, jogador->atq_especial.nome);
    printf("|                                                  |\n");
    printf("|--------------------------------------------------|\n");

    return(escolher_operacao(2, "seu ataque"));
}

int escolher_alvo(Inimigo *inimigos) {
    int idx_escolha;

    do {
        limpa_tela();
        printf("|--------------------------------|\n");
        for (int i = 0; i < 3; i++) {
            if (inimigos[i].vida_atual <= 0) {
                printf("|    %d. %s [Derrotado]         |\n", i + 1, inimigos[i].nome);
            } else {
                printf("|    %d. %s [Vida: %d/%d]         |\n", i + 1, inimigos[i].nome, inimigos[i].vida_atual, inimigos[i].vida_total);
            }
        }    
        printf("|--------------------------------|\n");

        idx_escolha = escolher_operacao(3, "o alvo do ataque") - 1;
        if (inimigos[idx_escolha].vida_atual <= 0) {
            printf("Este inimigo ja foi derrotado.\n");
            idx_escolha = FALHA;
        }
    } while (idx_escolha == FALHA);

    return idx_escolha;
}

int calcular_dano(PlayerBatalha* jogador, int ataque) {
    int dano = 0;
    if (ataque == BASICO) {
        dano = jogador->atq_basico.dano * jogador->dano_multiplicado;
        jogador->dano_multiplicado = 1;
        return dano;
    } else if (ataque == ESPECIAL) {
        dano = jogador->atq_especial.dano * jogador->dano_multiplicado;
        jogador->dano_multiplicado = 1;
        return dano;
    }
}

void atacar(PlayerBatalha* jogador, Inimigo *inimigos) {
    int alvo, dano_causado;
    
    if (escolha_ataque(jogador) == 1) {
        alvo = escolher_alvo(inimigos);
        dano_causado = calcular_dano(jogador, BASICO);
        inimigos[alvo].vida_atual -= dano_causado;
        printf("%s recebeu %d de dano!\n", inimigos[alvo].nome, dano_causado);
    } else {
        dano_causado = calcular_dano(jogador, ESPECIAL);
        for (int i = 0; i < 3; i++) {
            inimigos[i].vida_atual -= dano_causado;
        }
        printf("Todos os inimigos receberam %d de dano!\n", dano_causado);
    }
}

int usar_itens(Usuario* usuario_logado, PlayerBatalha* jogador) {
    int quantidade[QNT_ITENS_LOJA] = {0, 0, 0, 0, 0, 0};
    int itens_diferentes = 0, id_exibido = 1;;
    Item item_temp;

    // conta a quantidade de cada item no inventario
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        if (usuario_logado->consumiveis[i].ID != -1) {
            item_temp = usuario_logado->consumiveis[i];
            quantidade[item_temp.ID - 1]++;
        }
    }  

    // Verifica se tem item
    bool itens_presentes = false;
    for (int i = 0; i < QNT_ITENS_LOJA; i++) {
        if (quantidade[i] > 0) {
            itens_presentes = true;
            itens_diferentes++;
        }
    }

    if (!itens_presentes) {
        return FALHA;
    }

    limpa_tela();
    printf("|--------------------------------------------------|\n");
    printf("|                    INVENTARIO                    |\n");
    printf("|--------------------------------------------------|\n");
    printf("| %-3s %-20s %-10s %-12s |\n", "ID", "Nome", "Efeito", "Quantidade");
    printf("|--------------------------------------------------|\n");

    for (int i = 0; i < QNT_ITENS_LOJA; i++) {
        if (quantidade[i] > 0) {  // Exibe apenas os itens presentes no inventário
            for (int j = 0; j < QNT_CONSUMIVEIS; j++) {
                if (usuario_logado->consumiveis[j].ID == i + 1) {
                    item_temp = usuario_logado->consumiveis[j];
                    break;
                }
            }  

            // Define o efeito do item
            char efeito[50];
            if (item_temp.vida_recuperada > 0) {
                snprintf(efeito, sizeof(efeito), "Vida +%d%%", item_temp.vida_recuperada);
            } else if (item_temp.dano_aumentado > 0) {
                snprintf(efeito, sizeof(efeito), "Dano +%d%%", item_temp.dano_aumentado);
            } else {
                snprintf(efeito, sizeof(efeito), "Nenhum efeito");
            }

            // Exibe as informações formatadas
            printf("| %-3d %-20s %-10s %-12d |\n", 
            id_exibido, item_temp.nome, efeito, quantidade[item_temp.ID - 1]);
            id_exibido++;
        }
    }

    printf("|--------------------------------------------------|\n");
    
    int escolha;

    do {
        printf("> Digite 0 para voltar ou o ID do item para utilizar: ");
        if (scanf("%d", &escolha) != 1 || (escolha < 0 || escolha > itens_diferentes) ) {
            print_erro("Escolha invalida.\n\n");
            escolha = FALHA;
        }
        limpar_buffer();
    }while (escolha == FALHA);

    if (escolha == 0) {
        return OK;
    }
    id_exibido = 1; // Reinicia o contador
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        if (usuario_logado->consumiveis[i].ID != -1) {
            if (id_exibido == escolha) {
                item_temp = usuario_logado->consumiveis[i];

                if (item_temp.vida_recuperada > 0) {
                    int vida_recuperada = (item_temp.vida_recuperada * jogador->vida_base) / 100;
                    jogador->vida_atual += vida_recuperada;

                    // Garante que a vida não ultrapasse a vida_base
                    if (jogador->vida_atual > jogador->vida_base) {
                        jogador->vida_atual = jogador->vida_base;
                    }
                    limpa_tela();
                    printf("Voce recuperou %d de vida!\n", vida_recuperada);
                    delay(1000);
                }

                if (item_temp.dano_aumentado > 0) {
                    jogador->dano_multiplicado += (item_temp.dano_aumentado / 100.0);
                    limpa_tela();
                    printf("Voce aumentou seu dano em %d%%!\n", item_temp.dano_aumentado);
                    delay(1000);
                }

                usuario_logado->consumiveis[i].ID = -1;
                break;
            }
            id_exibido++;
        }
    }

    return OK;
}