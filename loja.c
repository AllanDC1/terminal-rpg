#include "terminalrpg.h"

//Funções e exibições da loja
void menu_loja () {
    limpa_tela();
    printf("Bem-vindo a Loja!\n\n");
    printf("1. Comprar itens\n");
    printf("2. Ver lista de itens\n");
    printf("3. Sair da loja\n");
}

void menu_itens_compraveis() {
    FILE *arquivo = abrir_arquivo("itens.txt", "r");
    if (arquivo == NULL) {
        print_erro("Erro ao abrir o arquivo para leitura dos itens.\n");
        return;
    }

    Item item;
    int id_desejado;

    // Cabeçalho da tabela
    printf("%-5s %-25s %-10s %-10s %-10s\n", "ID", "Nome", "Vida", "Força", "Preço");
    printf("---------------------------------------------------------------\n");

    // Leitura e exibição dos itens
    while (fscanf(arquivo, "%d,%254[^,],%d,%d,%d\n", 
            &item.ID, item.nome, 
            &item.vida_recuperada, 
            &item.dano_aumentado, 
            &item.preco) == 5) {
        // Exibe cada item em formato tabular
        printf("%-5d %-25s %-10d %-10d %-10d\n", 
            item.ID, item.nome, 
            item.vida_recuperada, 
            item.dano_aumentado, 
            item.preco);
    }

    fclose(arquivo);

    // pede o ID para o cara
    printf("\nDigite o ID da poção desejada: ");
    scanf("%d", &id_desejado);

    // vai atras do item
    arquivo = abrir_arquivo("itens.txt", "r");
    if (arquivo == NULL) {
        print_erro("Erro ao reabrir o arquivo para busca do item.\n");
        return;
    }

    // Busca o item pelo ID
    while (fscanf(arquivo, "%d,%254[^,],%d,%d,%d\n", 
            &item.ID, item.nome, 
            &item.vida_recuperada, 
            &item.dano_aumentado, 
            &item.preco) == 5) {
        if (item.ID == id_desejado) {
            printf("\nVocê escolheu a poção:\n");
            printf("ID: %d\n", item.ID);
            printf("Nome: %s\n", item.nome);
            printf("Vida Recuperada: %d\n", item.vida_recuperada);
            printf("Dano Aumentado: %d\n", item.dano_aumentado);
            printf("Preco: %d\n", item.preco);
            break;
        }
    }

    // ai precisa add o item no arquivo do usuario nn sei como vai fazer ainda com o arq do usario

    fclose(arquivo);
}

void criacao_arq_itens (){
    if (criar_arquivo("itens.txt") == OK) {
        printf("Arquivo criado com sucesso!\n");
    } else {
        printf("Falha ao criar o arquivo.\n");
    }

    Item lista_itens[] = {
        {1, "Pocao de Vida fraca", 25, 0, 100},
        {2, "Pocao de Vida media", 50, 0, 200},
        {3, "Pocao de Vida forte", 75, 0, 500},
        {4, "Pocao de forca Fraca", 10, 0, 100},
        {5, "Pocao de forca media", 25, 0, 200},
        {6, "Pocao de forca forte", 50, 0, 500},
    };

    FILE *arquivo = abrir_arquivo("itens.txt", "a");

    if (arquivo == NULL) {
        print_erro("Erro ao abrir o arquivo para adicionar itens.\n");
        return;
    }

    for (size_t i = 0; i < sizeof(lista_itens) / sizeof(lista_itens[0]); i++) {
        fprintf(arquivo, "%d;%s;%d;%d;%d\n", 
            lista_itens[i].ID, 
            lista_itens[i].nome, 
            lista_itens[i].vida_recuperada, 
            lista_itens[i].dano_aumentado, 
            lista_itens[i].preco
            );
    }

    fclose(arquivo);
}

int opcao_menu_loja () {
    char buffer[100];
    int opcao = 0, aprov = 0;
    
    do{
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