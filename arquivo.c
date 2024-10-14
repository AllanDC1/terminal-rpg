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

int ler_arquivo(const char* nome_arquivo, void **array, size_t tamanho_struct, bool bin) {
    FILE *fP;
    
    if (bin){
        fP = abrir_arquivo(nome_arquivo, "rb");
    } else {
        fP = abrir_arquivo(nome_arquivo, "r");
    }

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

int salvar_arquivo(const char* nome_arquivo, void *array, size_t tamanho_struct, size_t qnt_elementos, bool bin) {
    FILE *fP;
    
    if (bin) {
        fP = abrir_arquivo(nome_arquivo, "wb");
    } else {
        fP = abrir_arquivo(nome_arquivo, "w");
    }

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