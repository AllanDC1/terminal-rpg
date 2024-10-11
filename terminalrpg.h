#ifndef TERMINALRPG_H
#define TERMINALRPG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

enum {SAIDA = -2, FALHA = -1, OK = 0};  

typedef struct {
    int vida;
    int dano;
} Status;

typedef struct {
    int ID;
    char nome[255];
    int quantidade;
} Item;

typedef struct {
    int ID;
    char nome[255];
    int dano;
} Habilidade;

typedef struct {
    char nome_usuario[20];
    char senha[15];
    char nickname[30];
    double nivel;
    Item consumiveis[5];
    Habilidade inventario[10];
    Status status;
} Usuario;

typedef struct {
    int ID;
    char nome[255];
    int dificuldade;
    int qnt_moedas;
    Habilidade recompensas[5];
} Dungeon;

typedef struct {
    int ID;
    char nome[255];
    Status status;
    float qnt_xp;
} Inimigo;

// FUNCTIONS.C
void delay(int tempo_ms);
void limpar_buffer();
void print_erro(char *texto);
int iniciar_usuarios(Usuario **usuarios, int *qnt_usuarios);

// ARQUIVO.C
int criar_arquivo(const char* nome_arquivo);
FILE* abrir_arquivo(const char* nome_arquivo, const char* modo_abertura);
int ler_arquivo(const char* nome_arquivo, void **array, size_t tamanho_struct, bool bin);
int salvar_arquivo(const char* nome_arquivo, void *array, size_t tamanho_struct, size_t qnt_elementos, bool bin);

// MENU.C
int menu_inicial();
int escolher_operacao(int qnt_operacoes);

#endif