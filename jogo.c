#include "terminalrpg.h"

//Funções relacionadas ao funcionamento/mecânicas do jogo

// funcao para escrever o arquivo de batalha

void escrever_usuario_batalha(Usuario* usuario_logado) {
    Batalha usuario_batalha;

    // coloca no arquivo os dados do personagem
    usuario_batalha.ID = 0;
    strcpy(usuario_batalha.nome, usuario_logado->nickname);
    usuario_batalha.vida_base = usuario_logado->vida;
    usuario_batalha.vida_atual = usuario_logado->vida;
    usuario_batalha.dano_base = 1;  // ???? deixo quanto aki?
    usuario_batalha.dano_multiplicado = 1.00;

    FILE *arquivo_binario = fopen("batalhas.bin", "ab");
    if (arquivo_binario == NULL) {
        perror("Erro ao abrir o arquivo binário");
        return;
    }

    fwrite(&usuario_batalha, sizeof(Batalha), 1, arquivo_binario);
    fclose(arquivo_binario);
}

void escrever_inimigos_batalha(int id_dungeon, int camada) {
    Batalha batalhas[4]; // 1 | 3 -> 1 usuario e 3 inimigos
    Inimigo inimigo;

    int quantidade_inimigos = (camada == 4) ? 1 : 3; // 1 inimigo se for a camada 4 (boss), 3 nos outros casos

    FILE *arquivo_inimigos = fopen("inimigos.txt", "r");
    if (arquivo_inimigos == NULL) {
        perror("Erro ao abrir o arquivo de inimigos");
        return;
    }

    // cria um array para armazenar os inimigos da dungeon escolhida
    Inimigo inimigos_dungeon[4]; // coloca 1 ID em cada inimgo da dungeon
    int contador_inimigos = 0;

    // ve todos os inimigos e filtra pela dungeon escolhida
    while (fscanf(arquivo_inimigos, "%d \"%[^\"]\" %d %d %d", 
                  &inimigo.id_dungeon, 
                  inimigo.nome, 
                  &inimigo.vida, 
                  &inimigo.dano, 
                  &inimigo.nivel) == 5) {
        if (inimigo.id_dungeon == id_dungeon) {
            if (contador_inimigos < 4) {
                inimigos_dungeon[contador_inimigos++] = inimigo;
            }
        }
    }
    fclose(arquivo_inimigos);

    // add os inimigos a batalha com base na camada
    for (int i = 0; i < quantidade_inimigos; i++) {
        batalhas[i + 1].ID = i + 1; // IDs = 1 2 3 para inimigos

        if (camada == 1) {
            // Primeiro inimigo na camada 1
            batalhas[i + 1] = (Batalha){.ID = 1, .vida_base = inimigos_dungeon[0].vida, .vida_atual = inimigos_dungeon[0].vida, .dano_base = inimigos_dungeon[0].dano, .dano_multiplicado = 1.00};
            strcpy(batalhas[i + 1].nome, inimigos_dungeon[0].nome);
        } else if (camada == 2) {
            // Segundo inimigo na camada 2
            batalhas[i + 1] = (Batalha){.ID = 2, .vida_base = inimigos_dungeon[1].vida, .vida_atual = inimigos_dungeon[1].vida, .dano_base = inimigos_dungeon[1].dano, .dano_multiplicado = 1.00};
            strcpy(batalhas[i + 1].nome, inimigos_dungeon[1].nome);
        } else if (camada == 3) {
            // Terceiro inimigo na camada 3
            batalhas[i + 1] = (Batalha){.ID = 3, .vida_base = inimigos_dungeon[2].vida, .vida_atual = inimigos_dungeon[2].vida, .dano_base = inimigos_dungeon[2].dano, .dano_multiplicado = 1.00};
            strcpy(batalhas[i + 1].nome, inimigos_dungeon[2].nome);
        } else if (camada == 4) {
            // Último inimigo na camada 4 (boss)
            batalhas[1].ID = 1;
            batalhas[1].vida_base = inimigos_dungeon[3].vida;
            batalhas[1].vida_atual = inimigos_dungeon[3].vida;
            batalhas[1].dano_base = inimigos_dungeon[3].dano;
            strcpy(batalhas[1].nome, inimigos_dungeon[3].nome);
        }
    }

    FILE *arquivo_binario = fopen("batalhas.bin", "ab");
    if (arquivo_binario == NULL) {
        perror("Erro ao abrir o arquivo binário");
        return;
    }

    fwrite(batalhas + 1, sizeof(Batalha), quantidade_inimigos, arquivo_binario);
    fclose(arquivo_binario);
}

void escrever_arq_batalha(Usuario* usuario_logado, int id_dungeon, int camada) {

    escrever_usuario_batalha(usuario_logado);
    
    escrever_inimigos_batalha(id_dungeon, camada);
    
    printf("Batalhas escritas no arquivo binário com sucesso!\n");
}