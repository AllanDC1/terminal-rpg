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
    if (id_dungeon_escohlida == 0) {
        return SAIDA;
    }
    int idx_dungeon = id_dungeon_escohlida - 1;

    Inimigo *inimigos_dungeon;
    if (gerar_inimigos_dungeon(inimigos_dungeon, id_dungeon_escohlida, jogador.vida_base) == FALHA) {
        print_erro("Erro ao ler arquivo de inimigos.\n");
        return FALHA;
    }

    // Itera pelas camadas
    for (int i = 0; i < QNT_CAMADAS; i++) {        
        // Executa camada i        
        switch (combate_camada(usuario_logado, &jogador, dungeons[idx_dungeon], inimigos_dungeon, i)) {
            case FALHA: // erro nos arquivos
                print_erro("Cancelando combate...\n");
                voltar_menu();
                return FALHA; // cancela a dungeon
            case SAIDA: // fugir
                printf("Saindo da dungeon...\n");
                voltar_menu();
                return SAIDA; // escapa e cancela a dungeon
            case DERROTA:
                print_erro("Voce foi derrotado...\n");
                voltar_menu();
                return DERROTA; // encerra a dungeon
            case VITORIA:
                printf("Voce concluiu a camada %d!\n", i);
                enter_continuar();
                break;       
        }
    }    
    
    // na ultima camdada
    //exibir_combate_boss(jogador, lista_inimigos[3], dungeons[idx_dungeon].nome);
    
}

PlayerBatalha iniciar_jogador(Usuario *usuario_logado) {
    PlayerBatalha jogador = {
        .vida_base = usuario_logado->vida,
        .vida_atual = usuario_logado->vida,
        .atq_basico  = usuario_logado->atq_basico,
        .atq_especial = usuario_logado->atq_especial,
        .dano_multiplicado = 1.0,
        .cooldown_atq_especial = 0
    };
    strcpy(jogador.nick, usuario_logado->nickname);    

    return jogador;
}

int selecao_dungeon(Dungeon *array_dungeons) {
    limpa_tela();
    printf("Selecione a dungeon que deseja jogar:\n");
    exibir_dungeons(array_dungeons);
    
    return escolher_operacao(0, QNT_DUNGEONS, "a dungeon ou digite 0 para voltar");
}

int gerar_inimigos_dungeon(Inimigo *array_inimigos, int id_dungeon_escolhida, int vida_usuario) {
    Inimigo todos_inimigos[QNT_INIMIGOS];
    
    if (ler_arq_inimigos(todos_inimigos) == FALHA) {        
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

void gerar_inimigos_camada(Inimigo *inimigos_dungeon, Inimigo *inimigos_camada, int camada) {    
    if (camada != 3) {
        for (int i = 0; i < 3; i++) {            
            inimigos_camada[i] = inimigos_dungeon[rand() % 3]; // inimigo aleatorio 0 ou 1 ou 2 (nao inclui o boss)          
        }
    }
    else if (camada == 3) {
        inimigos_camada[0] = inimigos_dungeon[3]; // gera apenas o boss        
    }
}

void exibir_combate(PlayerBatalha jogador, Inimigo *inimigos, const char *nome_dungeon, int camada) {
    limpa_tela();
    // Exibir combate não boss
    if (camada != 3) {
        printf("                      %s %d\n", nome_dungeon, camada);        
        printf("--------------------------------------------------------------------\n");
        if (inimigos[0].vida_atual > 0) {
            printf("                              /    %s [Vida: %d/%d]\n", 
                inimigos[0].nome, inimigos[0].vida_atual, inimigos[0].vida_total);
        } else {
            printf("                              /    %s [Derrotado]\n", inimigos[0].nome);
        }

        printf("%s [Vida: %d/%d]    /    ", jogador.nick, jogador.vida_atual, jogador.vida_base);

        if (inimigos[1].vida_atual > 0) {
            printf("%s [Vida: %d/%d]\n", inimigos[1].nome, inimigos[1].vida_atual, inimigos[1].vida_total);
        } else {
            printf("%s [Derrotado]\n", inimigos[1].nome);
        }

        if (inimigos[2].vida_atual > 0) {
            printf("                            /    %s [Vida: %d/%d]\n", 
                inimigos[2].nome, inimigos[2].vida_atual, inimigos[2].vida_total);
        } else {
            printf("                            /    %s [Derrotado]\n", inimigos[2].nome);
        }
        printf("--------------------------------------------------------------------\n");
    }
    // Exibir combate boss
    else {
        printf("                     BOSS %s\n", nome_dungeon);    
        printf("--------------------------------------------------------------------\n");
        printf("                              /             \n");  // Linha vazia superior
        printf("%s [Vida: %d/%d]    /    %s [Vida: %d/%d]\n", 
                jogador.nick, jogador.vida_atual, jogador.vida_base,
                inimigos[0].nome, inimigos[0].vida_atual, inimigos[0].vida_total);
        printf("                            /             \n");  // Linha vazia inferior
        printf("--------------------------------------------------------------------\n");
    }
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

    return escolher_operacao(1, 3, "a sua acao");
}

int tentar_fuga() {    
    int tentativa = rand() % 100; // 0 a 99

    if (tentativa < 90) { // MUDAR PARA 24
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

    return(escolher_operacao(1, 2, "seu ataque"));
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

        idx_escolha = escolher_operacao(1, 3, "o alvo do ataque") - 1;
        if (inimigos[idx_escolha].vida_atual <= 0) {
            printf("Este inimigo ja foi derrotado.\n");
            delay(1500);
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

int atacar(PlayerBatalha* jogador, Inimigo *inimigos) {
    int alvo, dano_causado;
    
    if (escolha_ataque(jogador) == 1) {
        alvo = escolher_alvo(inimigos);
        dano_causado = calcular_dano(jogador, BASICO);
        inimigos[alvo].vida_atual -= dano_causado;
        printf("%s recebeu %d de dano!\n", inimigos[alvo].nome, dano_causado);        
    } else if (jogador->cooldown_atq_especial == 0) {
        dano_causado = calcular_dano(jogador, ESPECIAL);
        for (int i = 0; i < 3; i++) {
            inimigos[i].vida_atual -= dano_causado;
        }
        jogador->cooldown_atq_especial = 1; // coloquei como base 1 turno para usar novamente
        printf("Todos os inimigos receberam %d de dano!\n", dano_causado);
    } else {
        printf("Ataque especial em carregamento, espere mais %d turnos para usar novamente.", jogador->cooldown_atq_especial);
        return FALHA;
    }

    delay(2000);
    return OK;
}

int usar_itens(Usuario* usuario_logado, PlayerBatalha* jogador) {
    int id_escolhido;
    Item item_temp;

    limpa_tela();
    if (exibir_inventario(usuario_logado) == FALHA) {
        print_erro("Voce nao possui itens no inventario.\n");
        delay(1500);
        return SAIDA;
    }
    
    id_escolhido = escolher_operacao(0, QNT_ITENS_LOJA, "o ID do item ou 0 para voltar");

    if (id_escolhido == 0) {
        return SAIDA; // verificar
    }

    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        if (id_escolhido == usuario_logado->consumiveis[i].ID) {
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
            }

            if (item_temp.dano_aumentado > 0) {
                jogador->dano_multiplicado += (item_temp.dano_aumentado / 100.0);
                limpa_tela();
                printf("Voce aumentou seu dano em %d%%!\n", item_temp.dano_aumentado);
                
            }

            usuario_logado->consumiveis[i].ID = -1;
            delay(2000);
            return OK;           
        }        
    }
    print_erro("Voce nao possui o item com esse ID.\n");
    delay(2000);
    return SAIDA;
}

void dano_inimigos(PlayerBatalha* jogador, Inimigo *inimigos) {
    for (int i = 0; i < 3; i++) {
        if (inimigos[i].vida_atual > 0) {
            jogador->vida_atual -= inimigos[i].dano; // VER SE VAI TER ALGUM OUTRO FATOR QUE AFETARA O DANO
            printf("%s te inflingiu %d de dano!\n", inimigos[i].nome, inimigos[i].dano);
        }
    }
}

int combate_camada(Usuario *usuario_logado, PlayerBatalha* jogador, Dungeon dungeon, Inimigo *lista_inimigos_dungeon, int n_camada) {    
    Inimigo *inimigos_camada;
    int estado_combate;

    gerar_inimigos_camada(lista_inimigos_dungeon, inimigos_camada, n_camada);
      
    do {
        exibir_combate(*jogador, inimigos_camada, dungeon.nome, n_camada);
        // Turno do jogador
        switch (menu_combate()) {
            case 1:
                if (atacar(jogador, inimigos_camada) == FALHA) {
                    continue;
                } else {
                    break;
                }
            case 2:
                //usar item
                if (usar_itens(usuario_logado, jogador) == SAIDA){ 
                    continue;
                }
                break;
            case 3:
                if (tentar_fuga() == OK) {
                    print_sucesso("Voce conseguiu escapar.\n");
                    return SAIDA;
                } else {
                    print_erro("Voce falhou na fuga. Turno perdido.\n");
                    delay(2000);
                }
                break;
        }
        if (jogador->cooldown_atq_especial > 0) {jogador->cooldown_atq_especial -= 1;} // dps do turno dele o cooldown diminui caso seja maior q 0
        // Turno dos inimigos
        dano_inimigos(jogador, inimigos_camada);

        estado_combate = verificar_fim_combate(*jogador, inimigos_camada);

    } while (estado_combate == CONTINUAR);

    
    int xp_total = 0;

    if (estado_combate == VITORIA) {
        for (int i = 0; i == 3; i++) {
            xp_total += inimigos_camada->xp[&i];
        }
        // inimigos_camada->xp[i]
        
        // xp
        // verificacao nvl etc
    }

    return estado_combate;
}