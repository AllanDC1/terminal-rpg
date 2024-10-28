#include "../terminalrpg.h"

//Execução principal/inicial do programa

int main() {
    Usuario usuarios[MAX_USUARIOS], *usuario_logado;
    Habilidade habilidades[QNT_HABILIDADES];    

    int qnt_usuarios = 0;
    bool continuar = true;

    if (iniciar_sistema(usuarios, &qnt_usuarios, habilidades) == FALHA) {
        encerrar_sistema(usuarios, qnt_usuarios);
        return 1;
    }

    do{        
        printf("Bem-Vindo ao \033[0;31mTerminal RPG\033[0m!\n");
        // Menu Inicial
        switch (menu_inicial()) {
        case 1:
            if (login(usuarios, qnt_usuarios, &usuario_logado) == FALHA) {
                limpa_tela();
                continue;
            }
            // Menu Pos-Login
            do {
                limpa_tela();
                printf("Bem-Vindo, \033[1;35m%s\033[0m!\n", usuario_logado->nickname);
                switch (menu_principal()) {
                case 1:
                    // JOGAR
                    jogar(usuario_logado, habilidades);
                    break;
                case 2:
                    // INVENTARIO
                    menu_inventario(usuario_logado);
                    break;
                case 3:
                    // LOJA
                    menu_itens_compraveis(usuario_logado);
                    break;
                case 4:
                    // MODIFICAR CONTA
                    if (modificar_conta(usuarios, &qnt_usuarios, usuario_logado, habilidades) == SAIDA) {
                        continuar = false;
                    }            
                    break;
                case 0:
                    // SAIDA / LOGOUT
                    continuar = false;
                    break;
                }

            } while (continuar);
            continuar = true;
            limpa_tela();
            break;
        case 2:
            if (registro(usuarios, &qnt_usuarios, habilidades) == FALHA) {
                limpa_tela();
                continue;
            }
            limpa_tela();
            break;
        case 0:
            continuar = false;       
            break;
        }
    } while (continuar);

    encerrar_sistema(usuarios, qnt_usuarios);
    return 0;
}

