#include "terminalrpg.h"

//Funções relacionadas ao funcionamento/mecânicas do jogo

int selecao_dungeon(Dungeon *array_dungeons) {
    limpa_tela();
    printf("Selecione a dungeon que deseja jogar:");
    exibir_dungeons(array_dungeons);
    
    return escolher_operacao(QNT_DUNGEONS);
}