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

#define TAM_LOGIN 20
#define TAM_SENHA 15
#define TAM_NICK 30

#define MAX_CONSUMIVEIS 6

enum {SAIDA = -2, FALHA = -1, OK = 0};  

typedef struct {
    int vida;
    int dano;
} Status;

typedef struct {
    int ID;
    char nome[255];
    int vida_recuperada; // valor em porcentagem
    int dano_aumentado; // valor em porcentagem
    int preco;
} Item;

typedef struct {
    int ID;
    char nome[255];
    int dano;
} Habilidade;

typedef struct {
    char nome_usuario[TAM_LOGIN];
    char senha[TAM_SENHA];
    char nickname[TAM_NICK];
    double nivel;
    int moedas;
    Item consumiveis[MAX_CONSUMIVEIS];
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
void verificar_buffer(char *entrada);
void print_erro(char *texto);
void print_sucesso(char *texto);
void voltar_menu();
int iniciar_usuarios(Usuario **usuarios, int *qnt_usuarios);
void limpa_tela();

// ARQUIVO.C
int criar_arquivo(const char* nome_arquivo);
FILE* abrir_arquivo(const char* nome_arquivo, const char* modo_abertura);
int ler_arquivo(const char* nome_arquivo, void **array, size_t tamanho_struct, bool bin);
int salvar_arquivo(const char* nome_arquivo, void *array, size_t tamanho_struct, size_t qnt_elementos, bool bin);

// MENU.C
int escolher_operacao(int qnt_operacoes);
int menu_inicial();
int login(Usuario *array_usuarios, int qnt_usuarios, Usuario *usuario_logado);
int registro(Usuario *array_usuarios, int *qnt_usuarios);
int menu_principal();

// USUARIO.C
int validar_usuario(char *entrada_login, char *entrada_senha, Usuario *array_usuarios, int qnt_usuarios);
int validar_nome_usuario(char *entrada, Usuario *array_usuarios, int qnt_usuarios);
int validar_senha(char *entrada);
int validar_nickname(char *entrada);
void zerar_usuario(Usuario *usuario);

// LOJA.C
int menu_loja();
int menu_itens_compraveis(Usuario* usuario_logado);
int criacao_arq_itens();

#endif