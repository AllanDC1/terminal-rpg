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

int ler_arquivo_bin(const char* nome_arquivo, void **array, size_t tamanho_struct) {
    FILE *fP = abrir_arquivo(nome_arquivo, "rb");
    
    if (fP == NULL) {
        print_erro("Erro ao ler arquivo. Cancelando operacao...\n");
        return FALHA;
    }

    // Descobre tamanho do arquivo;
    fseek(fP, 0, SEEK_END);
    long tamanho_arquivo = ftell(fP);
    rewind(fP);

    // Descobre quantos elementos dos procurados o arquivo possui
    size_t qnt_elementos = tamanho_arquivo / tamanho_struct;
    // Aloca memoria para o array
    *array = malloc(qnt_elementos * tamanho_struct);

    // Le os elementos e salva no array
    size_t qnt_lidos = fread(*array, tamanho_struct, qnt_elementos, fP);
    if (qnt_lidos != qnt_elementos) {
        print_erro("Erro ao ler arquivo. Cancelando operacao...\n");
        free(*array);
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
        perror("Erro ao salvar arquivo. Cancelando operacao...\n");
        fclose(fP);
        return FALHA;
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

    int i = 0; // itens lidos

    while (i < QNT_CONSUMIVEIS && fscanf(fP, "%d %25s %d %d %d", 
        &array_itens[i].ID,
        array_itens[i].nome,
        &array_itens[i].vida_recuperada, 
        &array_itens[i].dano_aumentado, 
        &array_itens[i].preco) == 5) {  // Espera ler 5 campos por linha
        i++;
    }

    if (i != QNT_CONSUMIVEIS) {
        perror("Erro ao ler arquivo. Cancelando operacao...\n");
        fclose(fP);
        return FALHA;
    }

    fclose(fP);

    return i;
}

int criar_arq_itens(){

    Item lista_itens[] = {
        {0, "Pocao de Vida fraca", 25, 0, 100},
        {1, "Pocao de Vida media", 50, 0, 200},
        {2, "Pocao de Vida forte", 75, 0, 500},
        {3, "Pocao de forca Fraca", 0, 10, 100},
        {4, "Pocao de forca media", 0, 25, 200},
        {5, "Pocao de forca forte", 0, 50, 500},
    };

    FILE *fP = abrir_arquivo("itens.txt", "w");
    if (fP == NULL) {
        print_erro("Erro ao criar arquivo de itens.\n");
        return FALHA;
    }

    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        fprintf(fP, "%d %-20s %d %d %d\n", 
            lista_itens[i].ID,
            lista_itens[i].nome,
            lista_itens[i].vida_recuperada,
            lista_itens[i].dano_aumentado,
            lista_itens[i].preco
        );
    }

    fclose(fP);
    return OK;
}