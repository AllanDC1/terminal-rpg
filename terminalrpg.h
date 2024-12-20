#ifndef TERMINALRPG_H
#define TERMINALRPG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Limitações de tamanho dos dados do usuário
#define TAM_LOGIN 15
#define TAM_SENHA 15
#define TAM_NICK 15

#define MAX_USUARIOS 10

// Quantidade das mecânicas criadas 
#define QNT_CONSUMIVEIS 10
#define QNT_ITENS_LOJA 6
#define QNT_HABILIDADES 8
#define QNT_DUNGEONS 5
#define QNT_INIMIGOS 20
#define QNT_CAMADAS 3

// Respostas das funções para melhor legibilidade
enum {SAIDA = -2, FALHA = -1, OK = 0};
enum {VITORIA = 1, DERROTA = -3, CONTINUAR = 2};
enum {BASICO = 3, ESPECIAL = 4};

typedef struct {
    int ID;
    char nome[20];
    int vida_recuperada; // valor em porcentagem
    int dano_aumentado; // valor em porcentagem
    int preco;
} Item;

typedef struct {
    int ID;
    char nome[20];
    int dano;
    int requisito_xp;
} Habilidade;

typedef struct {
    char nome_usuario[TAM_LOGIN];
    char senha[TAM_SENHA];
    char nickname[TAM_NICK];
    int xp_usuario;
    int moedas;
    Item consumiveis[QNT_CONSUMIVEIS];
    Habilidade atq_basico;
    Habilidade atq_especial;
    int vida;
    int dificuldade;
} Usuario;

typedef struct {
    int ID;
    char nome[20];
    int dificuldade;
    int qnt_moedas;
} Dungeon;

typedef struct {
    int id_dungeon;
    char nome[20];
    int vida_total;
    int vida_atual;
    int dano;
    int xp;
} Inimigo;

typedef struct {
    char nick[TAM_NICK];
    int vida_base;
    int vida_atual;
    Habilidade atq_basico;
    Habilidade atq_especial;
    float dano_multiplicado;
    int cooldown_atq_especial;
} PlayerBatalha;

// FUNCTIONS.C
void delay(int tempo_ms);
void limpar_buffer();
void verificar_buffer(char *entrada);
void print_erro(char *texto);
void print_sucesso(char *texto);
void voltar_menu();
void enter_continuar();
int iniciar_usuarios(Usuario *usuarios, int *qnt_usuarios);
void limpa_tela();
int iniciar_sistema(Usuario *array_usuarios, int *qnt_usuarios, Habilidade *array_habilidades);
void encerrar_sistema(Usuario *array_usuarios, int qnt_usuarios);
int confirmar_acao();

// ARQUIVO.C
int criar_arquivo(const char* nome_arquivo);
FILE* abrir_arquivo(const char* nome_arquivo, const char* modo_abertura);
int ler_arquivo_bin(const char* nome_arquivo, void *array, size_t tamanho_struct);
int salvar_arquivo_bin(const char* nome_arquivo, void *array, size_t tamanho_struct, size_t qnt_elementos);
int criar_arq_itens();
int criar_arq_habilidades();
int ler_arq_itens(Item *array_itens);
int ler_arq_habilidades(Habilidade *array_habilidades);
int criar_arq_dungeons();
int criar_arq_inimigos();
int ler_arq_dungeons(Dungeon *array_dungeons);
int ler_arq_inimigos(Inimigo *array_inimigos);

// MENU.C
int escolher_operacao(int min_op, int max_op, char *texto);
int menu_inicial();
int menu_principal();
int menu_itens_compraveis(Usuario* usuario_logado);
int menu_inventario(Usuario* usuario_logado);
int modificar_conta(Usuario *array_usuarios, int *qnt_usuarios, Usuario *usuario_logado, Habilidade *array_habilidades);
void exibir_dungeons(Dungeon *array_dungeons);
int exibir_inventario(Usuario *usuario_logado);

// USUARIO.C
int validar_usuario(char *entrada_login, char *entrada_senha, Usuario *array_usuarios, int qnt_usuarios);
int validar_nome_usuario(char *entrada, Usuario *array_usuarios, int qnt_usuarios);
int validar_senha(char *entrada);
int validar_nickname(char *entrada);
void zerar_usuario(Usuario *usuario, Habilidade *habilidades);
int login(Usuario *array_usuarios, int qnt_usuarios, Usuario **usuario_logado);
int registro(Usuario *array_usuarios, int *qnt_usuarios, Habilidade *array_habilidades);
int alterar_apelido(Usuario *usuario_logado);
int excluir_conta(Usuario *array_usuarios, int *qnt_usuarios, Usuario *usuario_logado);

// JOGO.C 
int jogar(Usuario *usuario_logado, Habilidade *habilidades);
PlayerBatalha iniciar_jogador(Usuario *usuario_logado);
int selecao_dungeon(Dungeon *array_dungeons);
int gerar_inimigos_dungeon(Inimigo *array_inimigos, int id_dungeon_escolhida, int vida_usuario, int dificuldade_usuario);
void gerar_inimigos_camada(Inimigo *inimigos_dungeon, Inimigo *inimigos_camada, int camada);
void exibir_combate(PlayerBatalha jogador, Inimigo *inimigos, const char *nome_dungeon, int camada);
int verificar_fim_combate(PlayerBatalha jogador, Inimigo *inimigos, int qnt_inimigos);
int menu_combate();
int tentar_fuga();
int escolha_ataque(PlayerBatalha* jogador);
int escolher_alvo(Inimigo *inimigos, int qnt_inimigos);
int calcular_dano(PlayerBatalha* jogador, int ataque, int dano_base_multiplicado);
int atacar(PlayerBatalha* jogador, Inimigo *inimigos, int qnt_inimigos, int dano_base_multiplicado);
int usar_itens(Usuario* usuario_logado, PlayerBatalha* jogador);
void dano_inimigos(PlayerBatalha* jogador, Inimigo *inimigos, int qnt_inimigos);
int combate_camada(Usuario *usuario_logado, PlayerBatalha* jogador, Dungeon dungeon, Inimigo *lista_inimigos_dungeon, int n_camada, Habilidade *habilidades);
void verificar_nivel(Usuario *usuario_logado, PlayerBatalha *jogador, Habilidade *array_habilidades);

#endif