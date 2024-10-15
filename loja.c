#include "terminalrpg.h"

//Funções e exibições da loja
void menu_loja() {
    limpa_tela();
    printf("Bem-vindo a Loja!\n\n");
    printf("1. Comprar itens\n");
    printf("2. Ver lista de itens e habilidades\n");
    printf("3. Sair da loja\n");
}

void menu_itens_compraveis() {
    FILE *arquivo = abrir_arquivo("itens.bin", "rb");
    if (arquivo == NULL) {
        print_erro("Erro ao abrir o arquivo para leitura dos itens.\n");
        return;
    }

    Item item;
    int id_desejado;

    limpa_tela();

    // Cabeçalho da tabela
    printf("%-5s %-25s %-10s %-10s %-10s\n", "ID", "Nome", "Vida", "Forca", "Preco");
    printf("---------------------------------------------------------------\n");

    // Leitura e exibição dos itens
    while (fread(&item, sizeof(Item), 1, arquivo) == 1) {
        // Exibe cada item em formato tabular
        printf("%-5d %-25s %-10d %-10d %-10d\n", 
            item.ID, item.nome, 
            item.vida_recuperada, 
            item.dano_aumentado, 
            item.preco);
    }

    fclose(arquivo);

    // pede o ID para o cara
    printf("\nDigite o ID do item desejado: ");
    scanf("%d", &id_desejado);

    // Reabre o arquivo para busca do item
    arquivo = abrir_arquivo("itens.bin", "rb");
    if (arquivo == NULL) {
        print_erro("Erro ao reabrir o arquivo para busca do item.\n");
        return;
    }

    // Busca o item pelo ID
    bool item_encontrado = false;
    while (fread(&item, sizeof(Item), 1, arquivo) == 1) {
        if (item.ID == id_desejado) {
            printf("\nVocê escolheu a poção:\n");
            printf("ID: %d\n", item.ID);
            printf("Nome: %s\n", item.nome);
            printf("Vida Recuperada: %d\n", item.vida_recuperada);
            printf("Dano Aumentado: %d\n", item.dano_aumentado);
            printf("Preco: %d\n", item.preco);
            item_encontrado = true;
            break;  // Sai do loop ao encontrar o item
        }
    }

    if (!item_encontrado) {
        printf("Item com ID %d não encontrado.\n", id_desejado);
    }

    fclose(arquivo);

    // ai precisa add o item no arquivo do usuario nn sei como vai fazer ainda com o arq do usario

}

int criacao_arq_itens(){

    Item lista_itens[] = {
        {1, "Pocao de Vida fraca", 25, 0, 100},
        {2, "Pocao de Vida media", 50, 0, 200},
        {3, "Pocao de Vida forte", 75, 0, 500},
        {4, "Pocao de forca Fraca", 0, 10, 100},
        {5, "Pocao de forca media", 0, 25, 200},
        {6, "Pocao de forca forte", 0, 50, 500},
    };

    FILE *arquivo = abrir_arquivo("itens.bin", "wb");

    if (arquivo == NULL) {
        print_erro("Erro ao abrir o arquivo para adicionar itens.\n");
        return FALHA;
    }

    size_t qnt_itens = sizeof(lista_itens) / sizeof(lista_itens[0]);

    // Grava a lista no arq
    size_t elementos_escritos = fwrite(lista_itens, sizeof(Item), qnt_itens, arquivo);
    if (elementos_escritos != qnt_itens) {
        print_erro("Erro ao gravar os itens no arquivo.\n");
        fclose(arquivo);
        return FALHA;
    }

    fclose(arquivo);

    return OK;
}

int opcao_menu_loja() {
    char buffer[100];
    int opcao = 0, aprov = 0;
    
    do{
        limpa_tela();
        menu_loja();
        printf("> Escolha a opcao: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            opcao = atoi(buffer);
            if (opcao >= 1 && opcao <= 3){
                aprov = 1;
            }
        }
    }while (aprov == 0);
    
    return opcao;
}



// switch (opcao){
//         case 1:
//             // vai para o menu de comprar item
//             break;
        
//         case 2:
//             // vai para a lista de itens
//             break;
        
//         case 3:
//             // volta para o menu principal
//             break;

//         default:
//             break;
//     }