// #include "terminalrpg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//Funções e exibições da loja
void menu_loja () {
    printf("Bem-vindo a Loja!\n\n");
    printf("1. Comprar item\n");
    printf("2. Ver lista de itens\n");
    printf("3. Sair da loja\n");
}

void menu_comprar_item (){
    
    printf("Itens disponiveis\n");

}

int opcao_menu_loja () {
    int opcao = 0;
    
    do{
        // 
        menu_loja();
        scanf("%d", &opcao);
        if (scanf("%d", &opcao) != 1) { 
            printf("Entrada inválida! Digite um número.\n");
            while (getchar() != '\n');
            opcao = 0;
        }


    }while (opcao > 0 || opcao <= 3);
    
    
    

    switch (opcao){
        case 1:
            // vai para o menu de comprar item
            break;
        
        case 2:
            // vai para a lista de itens
            break;
        
        case 3:
            // volta para o menu principal
            break;

        default:
            break;
    }
}

int main() {
    opcao_menu_loja();

    return 1;
}