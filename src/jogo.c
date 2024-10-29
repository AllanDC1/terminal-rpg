#include "../terminalrpg.h"

//Funções relacionadas ao funcionamento/mecânicas do jogo

int jogar(Usuario *usuario_logado, Habilidade *habilidades) {
    PlayerBatalha jogador = iniciar_jogador(usuario_logado);
    Dungeon dungeons[QNT_DUNGEONS];
    Inimigo inimigos_dungeon[4]; // variedade de inimigos por dungeon
    
    if (ler_arq_dungeons(dungeons) == FALHA) {
        print_erro("Erro ao ler arquivo de dungeons.\n");
        return FALHA;        
    }

    int id_dungeon_escohlida = selecao_dungeon(dungeons);
    if (id_dungeon_escohlida == 0) {
        return SAIDA;
    }
    int idx_dungeon = id_dungeon_escohlida - 1;
    
    if (gerar_inimigos_dungeon(inimigos_dungeon, id_dungeon_escohlida, jogador.vida_base, usuario_logado->dificuldade) == FALHA) {
        print_erro("Erro ao ler arquivo de inimigos.\n");
        return FALHA;
    }

    // Itera pelas camadas
    for (int i = 1; i <= QNT_CAMADAS; i++) {        
        // Executa camada i        
        switch (combate_camada(usuario_logado, &jogador, dungeons[idx_dungeon], inimigos_dungeon, i, habilidades)) {
            case FALHA: // erro nos arquivos
                print_erro("Cancelando combate...\n");
                voltar_menu();
                return FALHA; // cancela a dungeon
            case SAIDA: // fugir
                printf("Saindo da dungeon...\n");
                voltar_menu();
                return SAIDA; // escapa e cancela a dungeon
            case DERROTA:
                limpa_tela();
                print_erro("Voce foi derrotado...\n");
                voltar_menu();
                return DERROTA; // encerra a dungeon
            case VITORIA:
                printf("\nVoce concluiu a camada %d!\n", i);
                enter_continuar();
                break;
        }
    }    
    limpa_tela();
    printf("\nParabens! Voce completou a dungeon %s!\n", dungeons[idx_dungeon].nome);
    if (id_dungeon_escohlida == 5) {
        usuario_logado->dificuldade += 1;
    }
    usuario_logado->moedas += dungeons[idx_dungeon].qnt_moedas;
    printf("Recompensa: %d Moedas!\n", dungeons[idx_dungeon].qnt_moedas);
    voltar_menu();

    return OK;
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
    
    return escolher_operacao(0, QNT_DUNGEONS, "o ID da dungeon ou digite 0 para voltar");
}

int gerar_inimigos_dungeon(Inimigo *array_inimigos, int id_dungeon_escolhida, int vida_usuario, int dificuldade_usuario) {
    Inimigo todos_inimigos[QNT_INIMIGOS];
    
    if (ler_arq_inimigos(todos_inimigos) == FALHA) {        
        return FALHA;    
    }

    int vida_dificuldade = 0;
    int dano = 1;

    if (dificuldade_usuario != 0) {
        vida_dificuldade = dificuldade_usuario * 100;
        dano = dificuldade_usuario + 1;
    }

    for (int i = 0; i < QNT_INIMIGOS; i++) {
        if (todos_inimigos[i].id_dungeon == id_dungeon_escolhida) {
            // Gerar os 4 primeiros monstros encontrados para a dungeon
            for (int j = 0; j < 4; j++) {
                array_inimigos[j] = todos_inimigos[i + j];
                
                // Ajustar a vida conforme a dificuldade
                array_inimigos[j].vida_total = array_inimigos[j].vida_total + vida_dificuldade;
                array_inimigos[j].vida_atual = array_inimigos[j].vida_total;
                array_inimigos[j].dano = array_inimigos[j].dano * dano;
                
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
        printf("                         %s %d\n", nome_dungeon, camada);        
        printf("--------------------------------------------------------------------------\n");
        if (inimigos[0].vida_atual > 0) {
            printf("                                    /    %s [Vida: %3d/%3d]\n", 
                inimigos[0].nome, inimigos[0].vida_atual, inimigos[0].vida_total);
        } else {
            printf("                                    /    %s [Derrotado]\n", inimigos[0].nome);
        }

        printf("%15s [Vida: %3d/%3d]    /      ", jogador.nick, jogador.vida_atual, jogador.vida_base);

        if (inimigos[1].vida_atual > 0) {
            printf("%s [Vida: %3d/%3d]\n", inimigos[1].nome, inimigos[1].vida_atual, inimigos[1].vida_total);
        } else {
            printf("%s [Derrotado]\n", inimigos[1].nome);
        }

        if (inimigos[2].vida_atual > 0) {
            printf("                                  /    %s [Vida: %3d/%3d]\n", 
                inimigos[2].nome, inimigos[2].vida_atual, inimigos[2].vida_total);
        } else {
            printf("                                  /    %s [Derrotado]\n", inimigos[2].nome);
        }
        printf("--------------------------------------------------------------------------\n");
    }
    // Exibir combate boss
    else {
        printf("                         %s BOSS\n", nome_dungeon);    
        printf("--------------------------------------------------------------------------\n");
        printf("                                    /             \n");  // Linha vazia superior
        printf("%15s [Vida: %3d/%3d]    /    %s [Vida: %3d/%3d]\n", 
                jogador.nick, jogador.vida_atual, jogador.vida_base,
                inimigos[0].nome, inimigos[0].vida_atual, inimigos[0].vida_total);
        printf("                                  /             \n");  // Linha vazia inferior
        printf("--------------------------------------------------------------------------\n");
    }
}

int verificar_fim_combate(PlayerBatalha jogador, Inimigo *inimigos, int qnt_inimigos) {
    if (jogador.vida_atual <= 0) {
        return DERROTA;
    }

    for (int i = 0; i < qnt_inimigos; i++) {
        if (inimigos[i].vida_atual <=0) {
            continue; // vai pra proxima iteracao
        } else {
            return CONTINUAR; // se qualquer inimigo estiver vivo
        }
    }
    // Se iterar por todos:
    return VITORIA;
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

    if (tentativa < 24) { // 25% de chance
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

int escolher_alvo(Inimigo *inimigos, int qnt_inimigos) {
    int idx_escolha;

    do {
        limpa_tela();        
        printf("|------------------------------------------|\n");
        
        for (int i = 0; i < qnt_inimigos; i++) {
            if (inimigos[i].vida_atual <= 0) {
                printf("| %2d. %-20s [Derrotado]     |\n", i + 1, inimigos[i].nome);
            } else {
                printf("| %2d. %-20s [Vida: %3d/%3d] |\n", i + 1, inimigos[i].nome, inimigos[i].vida_atual, inimigos[i].vida_total);
            }
        }

        printf("|------------------------------------------|\n");

        idx_escolha = escolher_operacao(1, qnt_inimigos, "o alvo do ataque") - 1;

        if (inimigos[idx_escolha].vida_atual <= 0) {
            printf("Este inimigo ja foi derrotado.\n");
            delay(1500);
            idx_escolha = FALHA;
        }
    } while (idx_escolha == FALHA);

    return idx_escolha;
}

int calcular_dano(PlayerBatalha* jogador, int ataque, int dano_base_multiplicado) {
    int dano = 0;
    if (ataque == BASICO) {
        dano = jogador->atq_basico.dano * jogador->dano_multiplicado * dano_base_multiplicado;
        jogador->dano_multiplicado = 1;
        return dano;
    } else if (ataque == ESPECIAL) {
        dano = jogador->atq_especial.dano * jogador->dano_multiplicado * dano_base_multiplicado;
        jogador->dano_multiplicado = 1;
        return dano;
    }
}

int atacar(PlayerBatalha* jogador, Inimigo *inimigos, int qnt_inimigos, int dano_base_multiplicado) {
    int alvo, dano_causado;
    
    if (escolha_ataque(jogador) == 1) {
        alvo = escolher_alvo(inimigos, qnt_inimigos);
        dano_causado = calcular_dano(jogador, BASICO, dano_base_multiplicado);
        inimigos[alvo].vida_atual -= dano_causado;
        limpa_tela();
        printf("\033[0;31m%s\033[0m recebeu %d de dano!\n", inimigos[alvo].nome, dano_causado);        
    } else if (jogador->cooldown_atq_especial == 0) {
        dano_causado = calcular_dano(jogador, ESPECIAL, dano_base_multiplicado);
        limpa_tela();
        for (int i = 0; i < qnt_inimigos; i++) {
            if (inimigos[i].vida_atual > 0) {
                inimigos[i].vida_atual -= dano_causado;
                printf("\033[0;31m%s\033[0m recebeu %d de dano!\n", inimigos[i].nome, dano_causado);
            }
        }
        jogador->cooldown_atq_especial = 2; // 1 para o turno atual, + 1 para o turno seguinte (turno sim, turno nao)
    } else {
        limpa_tela();
        printf("Ataque especial em carregamento, espere mais %d turno(s) para usar novamente.", jogador->cooldown_atq_especial);
        delay(2500);
        return FALHA;
    }

    enter_continuar();
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
        return SAIDA;
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

void dano_inimigos(PlayerBatalha* jogador, Inimigo *inimigos, int qnt_inimigos) {
    for (int i = 0; i < qnt_inimigos; i++) {
        if (inimigos[i].vida_atual > 0) {
            jogador->vida_atual -= inimigos[i].dano;
            printf("\033[0;31m%s\033[0m te inflingiu \033[0;31m%d\033[0m de dano!\n", inimigos[i].nome, inimigos[i].dano);
        }
    }
    enter_continuar();
}

int combate_camada(Usuario *usuario_logado, PlayerBatalha* jogador, Dungeon dungeon, Inimigo *lista_inimigos_dungeon, int n_camada, Habilidade *habilidades) {    
    Inimigo inimigos_camada[3];
    int estado_combate = CONTINUAR, qnt_inimigos;
    int dano_base_multiplicado = 1 + (usuario_logado->xp_usuario / 3500);

    gerar_inimigos_camada(lista_inimigos_dungeon, inimigos_camada, n_camada);
    if (n_camada == 3) {
        qnt_inimigos = 1;
    } else {
        qnt_inimigos = 3;
    }

    limpa_tela();
    printf("\n\033[0;33mEntrando na camada %d.\033[0m\n", n_camada); 
    delay(500);
    limpa_tela();
    printf("\n\033[0;33mEntrando na camada %d..\033[0m\n", n_camada); 
    delay(500);
    limpa_tela();
    printf("\n\033[0;33mEntrando na camada %d...\033[0m\n", n_camada); 
    delay(500);
      
    do {
        exibir_combate(*jogador, inimigos_camada, dungeon.nome, n_camada);
        // Turno do jogador
        switch (menu_combate()) {
            case 1:
                // Ataque
                if (atacar(jogador, inimigos_camada, qnt_inimigos, dano_base_multiplicado) == FALHA) {
                    continue;
                } else {
                    break;
                }
            case 2:
                // Usar item
                if (usar_itens(usuario_logado, jogador) == SAIDA){ 
                    continue;
                }
                break;
            case 3:
                // Fugir
                if (tentar_fuga() == OK) {
                    limpa_tela();
                    print_sucesso("Voce conseguiu escapar.\n");
                    return SAIDA;
                } else {
                    print_erro("Voce falhou na fuga. Turno perdido.\n");
                    delay(2000);
                }
                break;
        }
        if (jogador->cooldown_atq_especial > 0) {
            jogador->cooldown_atq_especial -= 1;
        } // dps do turno dele o cooldown diminui caso seja maior q 0

        // Verifica se os inimigos ainda estao vivos
        estado_combate = verificar_fim_combate(*jogador, inimigos_camada, qnt_inimigos);

        // Turno dos inimigos
        if (estado_combate == CONTINUAR) {
            limpa_tela();
            dano_inimigos(jogador, inimigos_camada, qnt_inimigos);
            // Verifica se o player esta vivo
            estado_combate = verificar_fim_combate(*jogador, inimigos_camada, qnt_inimigos);
        }

    } while (estado_combate == CONTINUAR);
    
    if (estado_combate == VITORIA) {
        int xp_total = 0;
        // Salva nivel ANTES de somar
        int nivel_anterior = usuario_logado->xp_usuario / 100;

        limpa_tela();
        // Calcula XP da camada
        for (int i = 0; i < qnt_inimigos; i++) {
            xp_total += inimigos_camada[i].xp; 
        }
        // Soma XP da camada no usuario
        usuario_logado->xp_usuario += xp_total;
        printf("Ganhou \033[1;34m%d\033[0m de XP!\n", xp_total);
        // Verifica se player subiu de nivel
        int nivel_atualizado = usuario_logado->xp_usuario / 100;
        if (nivel_atualizado > nivel_anterior) {
            printf("Subiu para o nivel \033[1;34m%d\033[0m!\n", nivel_atualizado);
        } 
        
        verificar_nivel(usuario_logado, jogador, habilidades);       
    }

    jogador->cooldown_atq_especial = 0; // Reseta o cooldown

    return estado_combate;
}

void verificar_nivel(Usuario *usuario_logado, PlayerBatalha *jogador, Habilidade *array_habilidades) {
    for (int i = 0; i < QNT_HABILIDADES; i++) {
        if (usuario_logado->xp_usuario >= array_habilidades[i].requisito_xp * 100) {
            if (strcmp(usuario_logado->atq_basico.nome, array_habilidades[i].nome) != 0 &&
                strcmp(usuario_logado->atq_especial.nome, array_habilidades[i].nome) != 0)
                {
                if (array_habilidades[i].ID % 2 == 0) { // id par = basico, id impar = especial
                    if (array_habilidades[i].ID > usuario_logado->atq_basico.ID) { // Confere se a habilidade eh melhor que a do usuario
                        usuario_logado->atq_basico = array_habilidades[i];
                        printf("Nova habilidade basica desbloqueada: \033[1;36m%s\033[0m!\n", array_habilidades[i].nome);
                    }
                } else {
                    if (array_habilidades[i].ID > usuario_logado->atq_especial.ID) {
                        usuario_logado->atq_especial = array_habilidades[i];
                        printf("Nova habilidade especial desbloqueada: \033[1;36m%s\033[0m!\n", array_habilidades[i].nome);
                    }
                }
            }
        }
    }
    jogador->atq_basico = usuario_logado->atq_basico;
    jogador->atq_especial = usuario_logado->atq_especial;

    // aumenta a vida do usuario conforme ele evolui de nivel
    int nivel_atual = usuario_logado->xp_usuario / 100;
    int vida_atualizada = 100 + (nivel_atual * 25);
    usuario_logado->vida = vida_atualizada;
    jogador->vida_base = vida_atualizada;
    
}