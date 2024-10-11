#include "terminalrpg.h"

//Execução principal/inicial do programa

int main() {
    Usuario *usuarios = NULL; // NULL para ainda nao alocar memoria
    int qnt_usuarios = 0;
    
    if (iniciar_usuarios(&usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Encerrando programa...\n");
        //logout() talvez
        return 0;
    }

    printf("Bem-Vindo ao Terminal RPG !\n");

    while(menu_inicial() != SAIDA);

    //logout()
    return 0;
}