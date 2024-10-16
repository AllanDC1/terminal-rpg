#include "terminalrpg.h"

//Execução principal/inicial do programa

int main() {
    Usuario usuarios[MAX_USUARIOS];
    Usuario usuario_logado;
    Habilidade habilidades[QNT_HABILIDADES];
    int qnt_usuarios = 0;
    bool continuar = true;

    if (iniciar_sistema(usuarios, &qnt_usuarios, habilidades) == FALHA) {
        encerrar_sistema(usuarios, qnt_usuarios);
        return 1;
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
                    // INVENTARIO
                    menu_inventario(&usuario_logado);
                    break;
                case 3:
                    // LOJA
                    menu_itens_compraveis(&usuario_logado);
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
            if (registro(usuarios, &qnt_usuarios, habilidades[0]) == FALHA) {
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

