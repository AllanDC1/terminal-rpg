#include "terminalrpg.h"

//Funções e exibições dos menus

int escolher_operacao(int qnt_operacoes) {
    int escolha;
    
    do {
        printf("> Escolha a operacao: ");
        if (scanf("%d", &escolha) != 1 || (escolha < 1 || escolha > qnt_operacoes) ) {
            print_erro("Operacao invalida.\n\n");
            escolha = FALHA;
        }
        limpar_buffer();
    }while (escolha == FALHA);

    return escolha;
}

int menu_inicial() {   
    printf("|---------------------------|\n");
    printf("|    1. Login               |\n");
    printf("|    2. Registrar-se        |\n");
    printf("|    3. Sair                |\n");
    printf("|---------------------------|\n");

    return escolher_operacao(3);
}