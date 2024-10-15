#include "terminalrpg.h"

//Funções e exibições da loja
void menu_loja() {
    limpa_tela();
    printf("Bem-vindo a Loja!\n\n");
    printf("1. Comprar itens\n");
    printf("2. Ver lista de itens e habilidades\n");
    printf("3. Sair da loja\n");
}

int menu_itens_compraveis() {
    Item *array_itens = NULL;
    int id_desejado, qnt_itens;
    
    qnt_itens = ler_arquivo("itens.bin", (void**)array_itens, sizeof(Item), true);
    if (qnt_itens == FALHA){
        print_erro("Erro ao ler os itens.\n");
        return FALHA;
    }

    limpa_tela();

    // Cabeçalho da tabela
    printf("%-5s %-25s %-10s %-10s %-10s\n", "ID", "Nome", "Vida", "Forca", "Preco");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < qnt_itens; i++) {
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
    while (scanf("%d", &id_desejado) != 1 || id_desejado < 0 || id_desejado > qnt_itens - 1) {
        print_erro("ID invalido. Insira novamente.\n");
        limpar_buffer();
    }
    limpar_buffer(); 

    for (int i = 0; i < qnt_itens; i++) {
        if (array_itens[i].ID == id_desejado) {
            printf("\nVocê escolheu a poção:\n");
            printf("ID: %d\n", array_itens[i].ID);
            printf("Nome: %s\n", array_itens[i].nome);
            printf("Vida Recuperada: %d\n", array_itens[i].vida_recuperada);
            printf("Dano Aumentado: %d\n", array_itens[i].dano_aumentado);
            printf("Preco: %d\n", array_itens[i].preco);
            break;  // Sai do loop ao encontrar o item
        }
    }

    // Pedir confirmacao talvez

    // ai precisa add o item no arquivo do usuario nn sei como vai fazer ainda com o arq do usario
    return OK;
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