#include "terminalrpg.h"

//Funções e exibições da loja
int menu_loja() {
    limpa_tela();
    printf("Bem-vindo a Loja!\n\n");
    printf("1. Comprar itens\n");
    printf("2. Ver lista de itens\n");
    printf("3. Sair da loja\n");

    return escolher_operacao(3);
}

int menu_itens_compraveis(Usuario* usuario_logado) {
    Item array_itens[QNT_CONSUMIVEIS];
    int id_desejado;
    
    if (ler_arq_itens(array_itens) == FALHA){
        print_erro("Erro ao ler os itens.\n");
        return FALHA;
    }

    limpa_tela();

    // Cabeçalho da tabela
    printf("%-5s %-25s %-10s %-10s %-10s\n", "ID", "Nome", "Vida", "Forca", "Preco");
    printf("---------------------------------------------------------------\n");
    // Printa lista de itens
    for (int i = 0; i < QNT_CONSUMIVEIS; i++) {
        printf("%-5d %-25s %-10d %-10d %-10d\n", 
            array_itens[i].ID,
            array_itens[i].nome, 
            array_itens[i].vida_recuperada, 
            array_itens[i].dano_aumentado, 
            array_itens[i].preco
        );
    }

    // Pede o ID do item
    printf("\nDigite o ID do item desejado: ");
    while (scanf("%d", &id_desejado) != 1 || id_desejado < 0 || id_desejado > QNT_CONSUMIVEIS - 1) {
        print_erro("ID invalido. Insira novamente.\n");
        limpar_buffer();
    }
    limpar_buffer();
    
    printf("\nVocê escolheu a poção:\n");
    printf("ID: %d\n", array_itens[id_desejado].ID);
    printf("Nome: %s\n", array_itens[id_desejado].nome);
    printf("Vida Recuperada: %d\n", array_itens[id_desejado].vida_recuperada);
    printf("Dano Aumentado: %d\n", array_itens[id_desejado].dano_aumentado);
    printf("Preco: %d\n", array_itens[id_desejado].preco);

    if (array_itens[id_desejado].preco > usuario_logado->moedas) {
        print_erro("Voce nao possui moedas suficientes para esse item.\n");
        return FALHA; // cancela compra
    }

    // Pedir confirmacao talvez

    usuario_logado->consumiveis[array_itens[id_desejado].ID] = array_itens[id_desejado];
    usuario_logado->moedas -= array_itens[id_desejado].preco;

    print_sucesso("Compra bem sucedida!\n");
    return OK;
}