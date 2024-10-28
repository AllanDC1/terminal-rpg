#include "../terminalrpg.h"

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
            return NULL; // retorna ponteiro como NULL
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
        {1, "Pocao de Vida Fraca", 50, 0, 100},
        {2, "Pocao de Vida Media", 75, 0, 200},
        {3, "Pocao de Vida Forte", 100, 0, 400},
        {4, "Pocao de Forca Fraca", 0, 50, 100},
        {5, "Pocao de Forca Media", 0, 100, 200},
        {6, "Pocao de Forca Forte", 0, 150, 400},
    };

    FILE *fP = abrir_arquivo("assets/itens.txt", "w");
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
        {0, "Espadada Fugaz", 5, 0},
        {1, "Corte em Arco", 8, 0},
        {2, "Golpe Flamejante", 12, 3},
        {3, "Bola de Fogo", 18, 6},
        {4, "Marretada Relampago", 25, 10},
        {5, "Raio do Julgamento", 29, 14},
        {6, "Soco Sismico", 40, 20},
        {7, "Terremoto", 60, 22}
    };

    FILE *fP = abrir_arquivo("assets/habilidades.txt", "w");
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

    FILE *fP = abrir_arquivo("assets/itens.txt", "r");
    
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

    FILE *fP = abrir_arquivo("assets/habilidades.txt", "r");
    
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
        {1, "Floresta do Leste", 1, 250},
        {2, "Montanhas Gelida", 2, 500},
        {3, "Caverna das Cinzas", 3, 750},
        {4, "Vale do Dragao", 4, 1000},    
        {5, "Fortaleza Sombria", 5, 2000}
    };

    FILE *fP = abrir_arquivo("assets/dungeons.txt", "w");
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
        {1, "Goblin", 10, 14, 3, 18},
        {1, "Lobo", 13, 13, 4, 17},
        {1, "Espirito Floral", 3, 3, 5, 18},
        {1, "General Ogro", 40, 40, 8, 60},
    //inimigos_montanha
        {2, "Elemental de Gelo", 18, 18, 6, 24},
        {2, "Arqueiro", 16, 16, 8, 25},
        {2, "Urso Polar", 22, 22, 7, 25},
        {2, "Golem Congelado", 60, 60, 12, 90},
    //inimigos_caverna
        {3, "Rato de Cinzas", 25, 25, 12, 33},
        {3, "Zumbi Carbonizado", 26, 26, 12, 32},
        {3, "Esqueleto Negro", 27, 27, 12, 33},
        {3, "Gargula das Chamas", 100, 100, 16, 120},
    //inimigos_vale
        {4, "Cultista Draconico", 29, 29, 15, 39},
        {4, "Lagarto", 31, 31, 17, 38},
        {4, "Principe Prateado", 33, 33, 17, 39},
        {4, "Deus Dragao", 190, 190, 19, 150},
    //inimigos_fortaleza
        {5, "Cavaleiro Imperial", 34, 34, 20, 50},
        {5, "Arqueiro Imperial", 38, 38, 22, 50},
        {5, "Golem Imperial", 65, 65, 12, 50},
        {5, "Lorde Roberto", 300, 300, 30, 250}
    };

    FILE *fP = fopen("assets/inimigos.txt", "w");
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
            lista_inimigos[i].xp
        );
    }

    fclose(fP);
    return OK;
}

int ler_arq_dungeons(Dungeon *array_dungeons) {    

    FILE *fP = abrir_arquivo("assets/dungeons.txt", "r");
    
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

    FILE *fP = abrir_arquivo("assets/inimigos.txt", "r");
    
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
                   &array_inimigos[i].xp) == 6) {
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
