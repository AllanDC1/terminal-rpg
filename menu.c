#include "terminalrpg.h"

//Funções e exibições dos menus

int menu_inicial() {   
    printf("|---------------------------|\n");
    printf("|    1. Login               |\n");
    printf("|    2. Registrar-se        |\n");
    printf("|    3. Sair                |\n");
    printf("|---------------------------|\n");

    switch (escolher_operacao(3)) {
        case 1:
            //login();
            return OK;
        case 2:
            //registro();
            return OK;
        case 3:
            return SAIDA;
    }
}

int escolher_operacao(int qnt_operacoes) {
    int escolha;
    
    do {
        printf("> Escolha a operacao: ");
        if (scanf("%d", &escolha) != 1 || (escolha < 1 || escolha > qnt_operacoes) ) {
            print_erro("Operacao invalida.\n\n");
            limpar_buffer();
            escolha = FALHA;
        }
    }while (escolha == FALHA);

    return escolha;
}