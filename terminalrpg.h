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

#endif