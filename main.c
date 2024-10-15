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

    if (criacao_arq_itens() == FALHA){
        print_erro("Encerrando programa...\n");
        //logout() talvez
        return 0;
    }

    do{
        limpa_tela();
        printf("Bem-Vindo ao Terminal RPG !\n");
        switch (menu_inicial()) {
        case 1:
            if (login(usuarios, qnt_usuarios, &usuario_logado) == FALHA) {
                continue;
            }
            // menu pos login

            do {
                limpa_tela();
                switch (menu_principal()) {
                case 1:
                    // batalha?
                    break;
                case 2:
                    // inventario?
                    break;
                
                case 3:
                    // loja
                    switch (opcao_menu_loja()){
                    case 1:
                        // comprar itens
                        menu_itens_compraveis();
                        break;
                    case 2:
                        // ver lista de itens
                        break;
                    case 3:
                        // voltar para o menu principal
                        break;
                    }
                    
                    break;

                case 4:
                    // sair?
                    // logout();
                    return 0;
                    break;
                }

            } while (true);

            break;

        case 2:
            if (registro(usuarios, &qnt_usuarios) == FALHA) {
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