#include "terminalrpg.h"

//Funções e exibições dos menus

int escolher_operacao(int qnt_operacoes) {
    int escolha;
    
    do {
        printf("> Escolha a operacao: ");
        if (scanf("%d", &escolha) != 1 || (escolha < 1 || escolha > qnt_operacoes) ) {
            print_erro("Operacao invalida.\n\n");
            escolha = FALHA;
        }
        limpar_buffer();
    }while (escolha == FALHA);

    return escolha;
}

int menu_inicial() {   
    printf("|---------------------------|\n");
    printf("|    1. Login               |\n");
    printf("|    2. Registrar-se        |\n");
    printf("|    3. Sair                |\n");
    printf("|---------------------------|\n");

    return escolher_operacao(3);
}

// funcoes para o menu principal do personagem provisorio

int menu_principal() {   
    printf("|---------------------------|\n");
    printf("|    1. Jogar               |\n");
    printf("|    2. Inventario          |\n");
    printf("|    3. Loja                |\n");
    printf("|    4. Modificar Conta     |\n");
    printf("|    5. Sair                |\n");
    printf("|---------------------------|\n");

    return escolher_operacao(5);
}

// MENU PARA LOJA

int menu_itens_compraveis(Usuario* usuario_logado) {
    Item array_itens[QNT_ITENS_LOJA];
    int id_desejado;
    
    if (ler_arq_itens(array_itens) == FALHA){
        print_erro("Erro ao ler os itens.\n");
        return FALHA;
    }

    limpa_tela();

    // Cabeçalho da tabela
    do {
        printf("BEM-VINDO A LOJA!\n");
        printf("---------------------------------------------------------------\n");
        printf("%-5s %-25s %-10s %-10s %-10s\n", "ID", "Nome", "Vida (%)", "Forca (%)", "Preco");
        printf("---------------------------------------------------------------\n");
        // Printa lista de itens
        for (int i = 0; i < QNT_ITENS_LOJA; i++) {
            printf("%-5d %-25s %-10d %-10d %-10d\n", 
                array_itens[i].ID,
                array_itens[i].nome, 
                array_itens[i].vida_recuperada, 
                array_itens[i].dano_aumentado, 
                array_itens[i].preco
            );
        }
        // Pede o ID do item
        printf("\nDigite 0 para sair ou ID do item desejado: ");
        if (scanf("%d", &id_desejado) != 1 || id_desejado < 0 || id_desejado > QNT_ITENS_LOJA) {
            print_erro("ID invalido. Insira novamente.\n");
            delay(1000);
            id_desejado = FALHA;
        }
        limpar_buffer();
        limpa_tela();
    } while (id_desejado == FALHA);    

    if (id_desejado == 0){
        return OK;
    } else {
        id_desejado -= 1; // id do array comeca do 0
    }

    printf("\nVoce escolheu a pocao:\n");
    printf("ID: %d\n", array_itens[id_desejado].ID);
    printf("Nome: %s\n", array_itens[id_desejado].nome);
    printf("Vida Recuperada: %d\n", array_itens[id_desejado].vida_recuperada);
    printf("Dano Aumentado: %d\n", array_itens[id_desejado].dano_aumentado);
    printf("Preco: %d\n", array_itens[id_desejado].preco);

    if (array_itens[id_desejado].preco > usuario_logado->moedas) {
        print_erro("\nVoce nao possui moedas suficientes para esse item.\n");
        voltar_menu();
        limpa_tela();
        return FALHA; // cancela compra
    }

    int idx_disp = FALHA; // indice do inventario que nao possui item
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        if (usuario_logado->consumiveis[i].ID == -1) {
            idx_disp = i;
            break;
        }        
    }

    if (idx_disp == FALHA) {
        print_erro("\nSeu inventario esta cheio.\n");
        voltar_menu();
        limpa_tela();
        return FALHA; // cancela a compra
    }

    if (confirmar_acao() == FALHA) {
        printf("Compra cancelada.\n");
        voltar_menu();
        return FALHA;
    }
    
    usuario_logado->consumiveis[idx_disp] = array_itens[id_desejado]; 
    usuario_logado->moedas -= array_itens[id_desejado].preco;

    print_sucesso("\nCompra bem sucedida!\n");
    voltar_menu();
    return OK;
}

// MENU PARA INVENTARIO

int menu_inventario(Usuario* usuario_logado) {
    limpa_tela();
    printf("|------------------------------|\n");
    printf("|  INVENTARIO DE %-14s|\n", usuario_logado->nickname);
    printf("|------------------------------|\n");
    printf("|  Nivel:  %-20.1f|\n", usuario_logado->nivel);
    printf("|  Moedas: %-20d|\n", usuario_logado->moedas);
    printf("|  Vida:   %-20d|\n", usuario_logado->vida);
    printf("|------------------------------|\n\n");

    int quantidade[QNT_ITENS_LOJA] = {0, 0, 0, 0, 0, 0};
    Item item_temp;

    // conta a quantidade de cada item no inventario
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        if (usuario_logado->consumiveis[i].ID != -1) {
            item_temp = usuario_logado->consumiveis[i];
            quantidade[item_temp.ID - 1]++;
        }
    }  

    // Verifica se tem item
    bool itens_presentes = false;
    for (int i = 0; i < QNT_ITENS_LOJA; i++) {
        if (quantidade[i] > 0) {
            itens_presentes = true;
            break;
        }
    }

    if (!itens_presentes) {
        printf("Nenhum item no inventario!\n");
        voltar_menu();
        return OK;
    }

    printf("| CONSUMIVEIS:                                     |\n");
    printf("|--------------------------------------------------|\n");
    printf("| %-3s %-20s %-10s %-12s |\n", "ID", "Nome", "Efeito", "Quantidade");
    printf("|--------------------------------------------------|\n");      

    for (int i = 0; i < QNT_ITENS_LOJA; i++) {
        if (quantidade[i] > 0) {  // Exibe apenas os itens presentes no inventário
            for (int j = 0; j < QNT_CONSUMIVEIS; j++) {  // interno
                if (usuario_logado->consumiveis[j].ID == i + 1) {
                    item_temp = usuario_logado->consumiveis[j];
                    break; // break no interno
                }
            }  

            // Define o efeito do item
            char efeito[50];
            if (item_temp.vida_recuperada > 0) {
                snprintf(efeito, sizeof(efeito), "Vida +%d%%", item_temp.vida_recuperada);
            } else if (item_temp.dano_aumentado > 0) {
                snprintf(efeito, sizeof(efeito), "Dano +%d%%", item_temp.dano_aumentado);
            } else {
                snprintf(efeito, sizeof(efeito), "Nenhum efeito");
            }

            // Exibe as informações formatadas
            printf("| %-3d %-20s %-10s %-12d |\n", 
            item_temp.ID, item_temp.nome, efeito, quantidade[item_temp.ID - 1]);
        }
    }

    printf("|--------------------------------------------------|\n");
    voltar_menu();

    return OK;
}

int modificar_conta(Usuario *array_usuarios, int *qnt_usuarios, Usuario *usuario_logado, Habilidade atq_inicial) {
    limpa_tela();
    printf("ALTERE DADOS DA SUA CONTA\n\n");
    printf("|---------------------------|\n");
    printf("|    1. Zerar Personagem    |\n");
    printf("|    2. Alterar Apelido     |\n");
    printf("|    3. Excluir Conta       |\n");
    printf("|    4. Sair                |\n");
    printf("|---------------------------|\n");

    switch (escolher_operacao(4))
    {
    case 1:
        if (confirmar_acao() == FALHA) {
            printf("Reinicio de personagem cancelada.\n");
            voltar_menu();
            return FALHA;
        }
        zerar_usuario(usuario_logado, atq_inicial);
        print_sucesso("Seu usuario foi reiniciado. Boa sorte em sua nova aventura!\n");
        voltar_menu();
        break;    
    case 2:
        if (alterar_apelido(usuario_logado) == OK) {
            print_sucesso("Seu apelido foi alterado!\n");
            voltar_menu();
        }
        break;    
    case 3:
        if (excluir_conta(array_usuarios, qnt_usuarios, usuario_logado) == SAIDA) {
            usuario_logado = NULL;
            print_sucesso("Sua conta foi excluida do sistema. Ate a proxima!\n");
            delay(2000);
            return SAIDA;
        }
        break;    
    case 4:        
        break;
    }

    return OK;
}