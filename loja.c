#include "terminalrpg.h"

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
    char buffer[100];
    int opcao = 0, aprov = 0;
    
    do{
        // 
        menu_loja();
        printf("> Escolha a opcao: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            opcao = atoi(buffer);
            if (opcao >= 1 && opcao <= 3){
                aprov = 1;
            }
        }


    }while (aprov == 0);
    
    return opcao;
}

// switch (opcao){
//         case 1:
//             // vai para o menu de comprar item
//             break;
        
//         case 2:
//             // vai para a lista de itens
//             break;
        
//         case 3:
//             // volta para o menu principal
//             break;

//         default:
//             break;
//     }