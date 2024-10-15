#include "terminalrpg.h"

//Funções e exibições da loja
void menu_loja () {
    limpa_tela();
    printf("Bem-vindo a Loja!\n\n");
    printf("1. Comprar itens\n");
    printf("2. Ver lista de itens\n");
    printf("3. Sair da loja\n");
}

void menu_comprar_item (){
    
    printf("Itens disponiveis\n");

}

void criacao_arq_itens (){
    if (criar_arquivo("itens.txt") == OK) {
        printf("Arquivo criado com sucesso!\n");
    } else {
        printf("Falha ao criar o arquivo.\n");
    }

    Item lista_itens[] = {
        {1, "Pocao de Vida fraca", 25, 0, 100},
        {1, "Pocao de Vida media", 50, 0, 200},
        {1, "Pocao de Vida forte", 75, 0, 500},
        {1, "Pocao de forca Fraca", 10, 0, 100},
        {1, "Pocao de forca media", 25, 0, 200},
        {1, "Pocao de forca forte", 50, 0, 500},
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