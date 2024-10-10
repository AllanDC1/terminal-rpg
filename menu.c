#include "terminalrpg.h"

//Funções e exibições dos menus

void menu_inicial() {   
    printf("|---------------------------|\n");
    printf("|    1. Login               |\n");
    printf("|    2. Registrar-se        |\n");
    printf("|    3. Sair                |\n");
    printf("|---------------------------|\n");

    switch (escolher_operacao(3)) {
        case 1:
            //login();
            break;
        case 2:
            //registro();
            break;
        case 3:
            //logout();
            break;
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