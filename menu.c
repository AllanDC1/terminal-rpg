#include "terminalrpg.h"

//Funções e exibições dos menus

int escolher_operacao(int min_op, int max_op, char *texto) {
    int escolha;
    
    do {
        printf("> Escolha %s: ", texto);
        if (scanf("%d", &escolha) != 1 || (escolha < min_op || escolha > max_op) ) {
            print_erro("Escolha invalida.\n\n");
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
    printf("|    0. Sair                |\n");
    printf("|---------------------------|\n");

    return escolher_operacao(0, 2, "a operacao");
}

// funcoes para o menu principal do personagem provisorio

int menu_principal() {   
    printf("|---------------------------|\n");
    printf("|    1. Jogar               |\n");
    printf("|    2. Inventario          |\n");
    printf("|    3. Loja                |\n");
    printf("|    4. Modificar Conta     |\n");
    printf("|    0. Sair                |\n");
    printf("|---------------------------|\n");

    return escolher_operacao(0, 4, "a operacao");
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
    printf("BEM-VINDO A LOJA!\n");
    printf("Suas moedas: %d\n", usuario_logado->moedas);
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
    printf("\n");
    // Pede o ID do item
    id_desejado = escolher_operacao(0, QNT_ITENS_LOJA, "o ID do item desejado ou 0 para sair"); 
    if (id_desejado == 0){
        return OK;
    } else {
        id_desejado -= 1; // id do array comeca do 0
    }

    limpa_tela();

    printf("\nVoce escolheu a pocao:\n");
    printf("-----------------------------------------------\n");
    printf("ID: %d\n", array_itens[id_desejado].ID);
    printf("Nome: %s\n", array_itens[id_desejado].nome);
    printf("Vida Recuperada: %d\n", array_itens[id_desejado].vida_recuperada);
    printf("Dano Aumentado: %d\n", array_itens[id_desejado].dano_aumentado);
    printf("Preco: %d\n", array_itens[id_desejado].preco);
    printf("-----------------------------------------------\n");

    printf("\nSuas moedas: %d\n", usuario_logado->moedas);

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
    printf("|  STATUS DE %-18s|\n", usuario_logado->nickname);
    printf("|------------------------------|\n");
    printf("|  Nivel:  %-20d|\n", usuario_logado->xp_usuario / 100);
    printf("|  Moedas: %-20d|\n", usuario_logado->moedas);
    printf("|  Vida:   %-20d|\n", usuario_logado->vida);
    printf("|------------------------------|\n\n");

    if (exibir_inventario(usuario_logado) == FALHA) {
        print_erro("Voce nao possui itens no inventario.\n");
        voltar_menu();
        return FALHA;
    }
    voltar_menu();

    return OK;
}

int modificar_conta(Usuario *array_usuarios, int *qnt_usuarios, Usuario *usuario_logado, Habilidade *array_habilidades) {
    limpa_tela();
    printf("ALTERE DADOS DA SUA CONTA\n");
    printf("|---------------------------|\n");
    printf("|    1. Zerar Personagem    |\n");
    printf("|    2. Alterar Apelido     |\n");
    printf("|    3. Excluir Conta       |\n");
    printf("|    0. Sair                |\n");
    printf("|---------------------------|\n");

    switch (escolher_operacao(0, 3, "a operacao"))
    {
    case 1:
        if (confirmar_acao() == FALHA) {
            printf("Reinicio de personagem cancelada.\n");
            voltar_menu();
            return FALHA;
        }
        zerar_usuario(usuario_logado, array_habilidades);
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
    case 0:        
        break;
    }

    return OK;
}

// Menu das opcoes da dungeon
void exibir_dungeons(Dungeon *array_dungeons) { 

    // printa as dungeons
    printf("|----------------------------------------------|\n");
    printf("| ID  Nome da Dungeon      Dificuldade  Moedas |\n");
    printf("|----------------------------------------------|\n");
    for (int j = 0; j < QNT_DUNGEONS; j++) {
        printf("| %-3d %-20s %-12d %-6d |\n",
        array_dungeons[j].ID, array_dungeons[j].nome, array_dungeons[j].dificuldade, array_dungeons[j].qnt_moedas);
        }
    printf("|----------------------------------------------|\n");
}

int exibir_inventario(Usuario *usuario_logado) {
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
        }
    }

    if (!itens_presentes) {
        return FALHA;
    }

    printf("|--------------------------------------------------|\n");
    printf("|                    INVENTARIO                    |\n");
    printf("|--------------------------------------------------|\n");
    printf("| %-3s %-20s %-10s %-12s |\n", "ID", "Nome", "Efeito", "Quantidade");
    printf("|--------------------------------------------------|\n");

    for (int i = 0; i < QNT_ITENS_LOJA; i++) {
        if (quantidade[i] > 0) {  // Exibe apenas os itens presentes no inventário
            for (int j = 0; j < QNT_CONSUMIVEIS; j++) {
                if (usuario_logado->consumiveis[j].ID == i + 1) {
                    item_temp = usuario_logado->consumiveis[j];
                    break;
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
    return OK;
}