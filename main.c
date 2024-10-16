#include "terminalrpg.h"

//Execução principal/inicial do programa

int main() {
    Usuario usuarios[MAX_USUARIOS];
    Usuario usuario_logado;
    int qnt_usuarios = 0;
    bool continuar = true;

    if (iniciar_usuarios(usuarios, &qnt_usuarios) == FALHA) {
        print_erro("Encerrando programa...\n");
        encerrar_sistema(usuarios, qnt_usuarios);
        return 0;
    }

    if (criar_arq_itens() == FALHA){
        print_erro("Encerrando programa...\n");
        encerrar_sistema(usuarios, qnt_usuarios);
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
            delay(1000);
            // menu pos login
            do {
                limpa_tela();
                printf("Bem-Vindo %s!\n", usuario_logado.nickname);
                switch (menu_principal()) {
                case 1:
                    // batalha?
                    break;
                case 2:
                    // inventario?
                    break;                
                case 3:
                    // loja
                    switch (menu_loja()){
                    case 1:
                        // comprar itens
                        menu_itens_compraveis(&usuario_logado);
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
                    // alterar dados conta                    
                    break;
                case 5:
                    continuar = false;
                    break;
                }

            } while (continuar);
            continuar = true;
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

    encerrar_sistema(usuarios, qnt_usuarios);
    return 0;
}