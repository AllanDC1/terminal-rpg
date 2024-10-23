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
    if (gerar_inimigos(lista_inimigos, id_dungeon_escohlida) == FALHA) {
        print_erro("Erro ao gerar inimigos da dungeon.\n");
        return FALHA;
    }

    // do while
    exibir_combate(jogador, lista_inimigos, dungeons[idx_dungeon].nome, 1);
    // na ultima camdada
    exibir_combate_boss(jogador, lista_inimigos[3], dungeons[idx_dungeon].nome);
}

PlayerBatalha iniciar_jogador(Usuario *usuario_logado) {
    PlayerBatalha jogador = {
        .vida_base = usuario_logado->vida,
        .vida_atual = usuario_logado->vida,
        .dano_basico = usuario_logado->atq_basico.dano,
        .dano_especial = usuario_logado->atq_especial.dano,
        .dano_multiplicado = 1.0
    };
    strcpy(jogador.nick, usuario_logado->nickname);    

    return jogador;
}

int selecao_dungeon(Dungeon *array_dungeons) {
    limpa_tela();
    printf("Selecione a dungeon que deseja jogar:");
    exibir_dungeons(array_dungeons);
    
    return escolher_operacao(QNT_DUNGEONS);
}

int gerar_inimigos(Inimigo *array_inimigos, int id_dungeon_escolhida) {
    Inimigo todos_inimigos[QNT_INIMIGOS];
    
    if (ler_arq_inimigos(todos_inimigos) == FALHA) {
        print_erro("Erro ao ler arquivo de inimigos.\n");
        return FALHA;    
    }

    for (int i = 0; i < QNT_INIMIGOS; i++) {
        if (todos_inimigos[i].id_dungeon == id_dungeon_escolhida) {
            for (int j = 0; j < 4; j++) {
                array_inimigos[j] = todos_inimigos[i + j];
            }
            break;
        }
    }

    return OK;
} 

void exibir_combate(PlayerBatalha jogador, Inimigo *inimigos, const char *nome_dungeon, int camada) {
    printf("               %s %d\n", nome_dungeon, camada);
    
    printf("-------------------------------------------\n");
    printf("                     /    (%s) %s\n", inimigos[0].nome, 
            (inimigos[0].vida_atual > 0) ? "[Vida: %d/%d]" : "Derrotado", 
            inimigos[0].vida_atual, inimigos[0].vida_total);
    printf("(%s) [Vida: %d/%d]    /    (%s) %s\n", 
            jogador.nick, jogador.vida_atual, jogador.vida_base,
            inimigos[1].nome, (inimigos[1].vida_atual > 0) ? "[Vida: %d/%d]" : "Derrotado", 
            inimigos[1].vida_atual, inimigos[1].vida_total);
    printf("                     /    (%s) %s\n", inimigos[2].nome, 
            (inimigos[2].vida_atual > 0) ? "[Vida: %d/%d]" : "Derrotado", 
            inimigos[2].vida_atual, inimigos[2].vida_total);    
    printf("-------------------------------------------\n");
}

void exibir_combate_boss(PlayerBatalha jogador, Inimigo boss, const char *nome_dungeon) {
    printf("               %s\n", nome_dungeon);
    
    printf("-------------------------------------------\n");
    printf("                     \n");  // Linha vazia superior
    printf("(%s) [Vida: %d/%d]    /    (%s) %s\n", 
            jogador.nick, jogador.vida_atual, jogador.vida_base,
            boss.nome, 
            (boss.vida_atual > 0) ? "[Vida: %d/%d]" : "Derrotado", 
            boss.vida_atual, boss.vida_total);
    printf("                     \n");  // Linha vazia inferior
    printf("-------------------------------------------\n");
}