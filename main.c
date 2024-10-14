#include "terminalrpg.h"

//Execução principal/inicial do programa

int main() {
    Usuario *usuarios = NULL; // NULL para ainda nao alocar memoria
    Usuario usuario_logado;
    int qnt_usuarios = 0;
    bool continuar = true;
    
    if (iniciar_usuarios(&usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Encerrando programa...\n");
        //logout() talvez
        return 0;
    }

    printf("Bem-Vindo ao Terminal RPG !\n");
    
    do{
        switch (menu_inicial()) {
        case 1:
            if (login(usuarios, qnt_usuarios, &usuario_logado) == FALHA) {
                continue;
            }
            // menu pos login
            break;
        case 2:
            if (registro(usuarios, qnt_usuarios) == FALHA) {
                continue;
            }
            break;
        case 3:
            continuar = false;       
            break;
        }
    } while (continuar);

    //logout()
    return 0;
}