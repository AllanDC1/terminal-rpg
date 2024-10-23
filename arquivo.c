#include "terminalrpg.h"

//Funções relacionadas a criação e alteração de arquivos

int criar_arquivo(const char* nome_arquivo) {
    FILE *ponteiro_arquivo = fopen(nome_arquivo, "w");

    if (ponteiro_arquivo == NULL) {
        print_erro("Erro ao criar arquivo.\n");
        return FALHA;
    }

    fclose(ponteiro_arquivo);
    return OK;
}

FILE* abrir_arquivo(const char* nome_arquivo, const char* modo_abertura) {
    FILE *ponteiro_arquivo = fopen(nome_arquivo, modo_abertura);

    if (ponteiro_arquivo == NULL) {
        if (criar_arquivo(nome_arquivo) == FALHA) {
            print_erro("Erro na abertura do arquivo.\n");
            return NULL; // retorna como NULL
        }
        ponteiro_arquivo = fopen(nome_arquivo, modo_abertura); 
    }
    
    return ponteiro_arquivo;
}

int ler_arquivo_bin(const char* nome_arquivo, void *array, size_t tamanho_struct) {
    FILE *fP = abrir_arquivo(nome_arquivo, "rb");
    
    if (fP == NULL) {
        print_erro("Erro ao ler arquivo. Cancelando operacao...\n");
        return FALHA;
    }

    // Descobre tamanho do arquivo;
    fseek(fP, 0, SEEK_END);
    long tamanho_arquivo = ftell(fP);
    rewind(fP);

    // Descobre quantos elementos o arquivo possui
    size_t qnt_elementos_arquivo = tamanho_arquivo / tamanho_struct;
    // Le os elementos e salva no array
    size_t qnt_lidos = fread(array, tamanho_struct, qnt_elementos_arquivo, fP);
    if (qnt_lidos != qnt_elementos_arquivo) {
        print_erro("Erro ao ler arquivo. Cancelando operacao...\n");
        fclose(fP);
        return FALHA;
    }
    fclose(fP);

    return (int) qnt_lidos;
}

int salvar_arquivo_bin(const char* nome_arquivo, void *array, size_t tamanho_struct, size_t qnt_elementos) {
    FILE *fP = abrir_arquivo(nome_arquivo, "wb");
   
    if (fP == NULL) {
        print_erro("Erro ao ler arquivo. Cancelando operacao...\n");
        return FALHA;
    }
    
    size_t qnt_escritos = fwrite(array, tamanho_struct, qnt_elementos, fP);
    if (qnt_escritos != qnt_elementos) {
        print_erro("Erro ao salvar arquivo. Cancelando operacao...\n");
        fclose(fP);
        return FALHA;
    }

    fclose(fP);
    return OK;
}

int criar_arq_itens(){


    Item lista_itens_criar[] = {
        {1, "Pocao de Vida fraca", 25, 0, 100},
        {2, "Pocao de Vida media", 50, 0, 200},
        {3, "Pocao de Vida forte", 75, 0, 500},
        {4, "Pocao de forca Fraca", 0, 10, 100},
        {5, "Pocao de forca media", 0, 25, 200},
        {6, "Pocao de forca forte", 0, 50, 500},
    };

    FILE *fP = abrir_arquivo("itens.txt", "w");
    if (fP == NULL) {
        print_erro("Erro ao criar arquivo de itens.\n");
        return FALHA;
    }

    for (int i = 0; i < QNT_ITENS_LOJA; i++) {
        fprintf(fP, "%d \"%s\" %d %d %d\n", 
            lista_itens_criar[i].ID,
            lista_itens_criar[i].nome,
            lista_itens_criar[i].vida_recuperada,
            lista_itens_criar[i].dano_aumentado,
            lista_itens_criar[i].preco
        );
    }

    fclose(fP);
    return OK;
}

int criar_arq_habilidades(){

    Habilidade lista_habilidades[] = {
        {0, "Corte fugaz", 5, 0},
        {1, "Bola de fogo", 3, 5},
        {2, "Golpe flamejante", 10, 10},
        {3, "Raio do julgamento", 7, 15},
        {4, "Marretada relampago", 15, 20},
        {5, "Terremoto", 12, 25},
        {6, "Soco sismico", 20, 30}
    };

    FILE *fP = abrir_arquivo("habilidades.txt", "w");
    if (fP == NULL) {
        print_erro("Erro ao criar arquivo de habilidades.\n");
        return FALHA;
    }

    for (int i = 0; i < QNT_HABILIDADES; i++) {
        fprintf(fP, "%d \"%s\" %d %d\n", 
            lista_habilidades[i].ID,
            lista_habilidades[i].nome,
            lista_habilidades[i].dano,
            lista_habilidades[i].requisito_xp
        );
    }

    fclose(fP);
    return OK;
}

int ler_arq_itens(Item *array_itens) {    

    FILE *fP = abrir_arquivo("itens.txt", "r");
    
    if (fP == NULL) {
        print_erro("Erro ao ler arquivo. Cancelando operacao...\n");
        return FALHA;
    }

    char linha[100]; //buffer
    int i = 0; // itens lidos

    while (i < QNT_ITENS_LOJA && fgets(linha, sizeof(linha), fP) != NULL) {
        if (sscanf(linha, "%d \"%[^\"]\" %d %d %d", 
            &array_itens[i].ID,
            array_itens[i].nome,
            &array_itens[i].vida_recuperada, 
            &array_itens[i].dano_aumentado, 
            &array_itens[i].preco) == 5) {
            i++; 
        } else {
            print_erro("Erro ao processar linha de itens.\n");
            fclose(fP);
            return FALHA;
        }
    }

    if (i != QNT_ITENS_LOJA) {
        print_erro("Erro ao ler arquivo de itens. Cancelando operacao...\n");
        fclose(fP);
        return FALHA;
    }

    fclose(fP);

    return i;
}

int ler_arq_habilidades(Habilidade *array_habilidades) {    

    FILE *fP = abrir_arquivo("habilidades.txt", "r");
    
    if (fP == NULL) {
        print_erro("Erro ao abrir arquivo de habilidades. Cancelando operacao...\n");
        return FALHA;
    }

    char linha[100]; // buffer
    int i = 0; // itens lidos

    while (i < QNT_HABILIDADES && fgets(linha, sizeof(linha), fP) != NULL) {
        if (sscanf(linha, "%d \"%[^\"]\" %d %d", 
                   &array_habilidades[i].ID,
                   array_habilidades[i].nome,
                   &array_habilidades[i].dano, 
                   &array_habilidades[i].requisito_xp) == 4) {
            i++; // Incrementa contador de habilidades lidas
        } else {
            print_erro("Erro ao processar linha de habilidades.\n");
            fclose(fP);
            return FALHA;
        }
    }

    if (i != QNT_HABILIDADES) {
        print_erro("Erro ao ler arquivo de habilidades. Cancelando operacao...\n");
        fclose(fP);
        return FALHA;
    }

    fclose(fP);

    return i;
}

int criar_arq_dungeons() {

    Dungeon lista_dungeons[] = {
        {1, "Floresta do Leste", 1, 150},
        {2, "Montanha Gelida", 2, 300},
        {3, "Caverna das Cinzas", 3, 500},
        {4, "Vale do Dragao", 4, 750},    
        {5, "Fortaleza Sombria", 5, 1000}
    };

    FILE *fP = abrir_arquivo("dungeons.txt", "w");
    if (fP == NULL) {
        print_erro("Erro ao criar arquivo de dungeons.\n");
        return FALHA;
    }

    for (int i = 0; i < QNT_DUNGEONS; i++) {
        fprintf(fP, "%d \"%s\" %d %d\n", 
            lista_dungeons[i].ID,
            lista_dungeons[i].nome,
            lista_dungeons[i].dificuldade,
            lista_dungeons[i].qnt_moedas
        );
    }

    fclose(fP);
    return OK;
}

int criar_arq_inimigos() {

    Inimigo lista_inimigos[] = {
    //inimigos_floresta
        {1, "Goblin", 12, 12, 3, 1},
        {1, "Lobo", 18, 18, 4, 1},
        {1, "Espirito Floral", 22, 22, 6, 3},
        {1, "General Ogro", 40, 40, 8, 5},
    //inimigos_montanha
        {2, "Elemental de Gelo", 12, 12, 3, 2},
        {2, "Arqueiro", 18, 18, 4, 2},
        {2, "Urso Polar", 25, 25, 7, 4},
        {2, "Golem Congelado", 45, 45, 10, 6},
    //inimigos_caverna
        {3, "Rato de Cinzas", 12, 12, 3, 2},
        {3, "Zumbi Carbonizado", 20, 20, 5, 3},
        {3, "Esqueleto Negro", 22, 22, 6, 4},
        {3, "Gargula das Chamas", 50, 50, 10, 6},
    //inimigos_vale
        {4, "Cultista Draconico", 12, 12, 4, 2},
        {4, "Lagarto", 20, 20, 5, 3},
        {4, "Principe Prateado", 25, 25, 7, 5},
        {4, "Deus Dragao", 55, 55, 12, 7},
    //inimigos_fortaleza
        {5, "Cavaleiro", 15, 15, 4, 3},
        {5, "Corvo Estranho", 20, 20, 5, 3},
        {5, "Espectro Abissal", 25, 25, 7, 5},
        {5, "Lorde Roberto", 60, 60, 12, 8}
    };

    FILE *fP = fopen("inimigos.txt", "w");
    if (fP == NULL) {
        printf("Erro ao criar arquivo de inimigos.\n");
        return FALHA;
    }

    for (int i = 0; i < 4 * QNT_DUNGEONS; i++) {
        fprintf(fP, "%d \"%s\" %d %d %d %d\n", 
            lista_inimigos[i].id_dungeon,
            lista_inimigos[i].nome,
            lista_inimigos[i].vida_total,
            lista_inimigos[i].vida_atual,
            lista_inimigos[i].dano,
            lista_inimigos[i].nivel
        );
    }

    fclose(fP);
    return OK;
}

int ler_arq_dungeons(Dungeon *array_dungeons) {    

    FILE *fP = abrir_arquivo("dungeons.txt", "r");
    
    if (fP == NULL) {
        print_erro("Erro ao abrir arquivo de dungeons. Cancelando operacao...\n");
        return FALHA;
    }

    char linha[100]; // buffer
    int i = 0; // itens lidos

    while (i < QNT_DUNGEONS && fgets(linha, sizeof(linha), fP) != NULL) {
        if (sscanf(linha, "%d \"%[^\"]\" %d %d", 
                   &array_dungeons[i].ID,
                   array_dungeons[i].nome,
                   &array_dungeons[i].dificuldade, 
                   &array_dungeons[i].qnt_moedas) == 4) {
            i++;
        } else {
            print_erro("Erro ao processar linha de dungeons.\n");
            fclose(fP);
            return FALHA;
        }
    }

    if (i != QNT_DUNGEONS) {
        print_erro("Erro ao ler arquivo de dungeons. Cancelando operacao...\n");
        fclose(fP);
        return FALHA;
    }

    fclose(fP);

    return i;
}

int ler_arq_inimigos(Inimigo *array_inimigos) {    

    FILE *fP = abrir_arquivo("inimigos.txt", "r");
    
    if (fP == NULL) {
        print_erro("Erro ao abrir arquivo de dungeons. Cancelando operacao...\n");
        return FALHA;
    }

    char linha[100]; // buffer
    int i = 0; // itens lidos

    while (i < QNT_INIMIGOS && fgets(linha, sizeof(linha), fP) != NULL) {
        if (sscanf(linha, "%d \"%[^\"]\" %d %d %d %d", 
                   &array_inimigos[i].id_dungeon,
                   array_inimigos[i].nome,
                   &array_inimigos[i].vida_total, 
                   &array_inimigos[i].vida_atual, 
                   &array_inimigos[i].dano,
                   &array_inimigos[i].nivel) == 6) {
            i++;
        } else {
            print_erro("Erro ao processar linha de inimigos.\n");
            fclose(fP);
            return FALHA;
        }
    }

    if (i != QNT_INIMIGOS) {
        print_erro("Erro ao ler arquivo de inimigos. Cancelando operacao...\n");
        fclose(fP);
        return FALHA;
    }

    fclose(fP);

    return i;
}