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

    // Pedir confirmacao talvez

    usuario_logado->consumiveis[array_itens[id_desejado].ID] = array_itens[id_desejado];
    usuario_logado->moedas -= array_itens[id_desejado].preco;    

    print_sucesso("Compra bem sucedida!\n");
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

    int quantidade[QNT_CONSUMIVEIS] = {0};

    // Verifica se tem item
    int itens_presentes = 0;
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        if (quantidade[i] > 0) {
            itens_presentes = 1;
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
    printf("| %-3s %-16s %-16s %-10s |\n", "ID", "Nome", "Efeito", "Quantidade");
    printf("|--------------------------------------------------|\n");

    // conta a quantidade de cada item no inventario
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        if (usuario_logado->consumiveis[i].ID != 0) {
            Item item = usuario_logado->consumiveis[i];
            quantidade[item.ID]++;
        }
    }    

    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        if (quantidade[i] > 0) {  // Exibe apenas os itens presentes no inventário
            Item item = usuario_logado->consumiveis[i];

            // Define o efeito do item
            char efeito[50];
            if (item.vida_recuperada > 0) {
                snprintf(efeito, sizeof(efeito), "Vida +%d%%", item.vida_recuperada);
            } else if (item.dano_aumentado > 0) {
                snprintf(efeito, sizeof(efeito), "Dano +%d%%", item.dano_aumentado);
            } else {
                snprintf(efeito, sizeof(efeito), "Nenhum efeito");
            }

            // Exibe as informações formatadas
            printf("| %-3d %-16s %-16s %-10d |\n", 
            item.ID, item.nome, efeito, quantidade[item.ID]);
        }
    }

    printf("|--------------------------------------------------|\n");
    voltar_menu();

    return OK;
}