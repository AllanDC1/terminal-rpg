# Projeto 2-Terminal RPG (2° Sem. CC FEI)

## Objetivo do Projeto
Projeto desenvolvido na linguagem C, por alunos no 2° semestre do curso de Ciência da Computação da FEI. <br> <br>
A ideia do projeto se trata de um jogo simples de RPG, executado no terminal, onde os jogadores passarão por dungeons, 
enfrentando inimigos em combates por turno, subindo de nível e ganhando novas habilidades. <br> <br>
Além disso o sistema conta com registro e login para os usuários operarem pelo sistema, e outra funções como modificar dados da sua conta, e uma loja para comprar itens e verifica-los no inventário.

## Funcionalidades do usuário

### Menu inicial

1. **Login** | Usuário informa login (nome de usuário) e senha.
2. **Registro** | Usuário informa nome de usuário, senha e apelido (mostrado dentro do jogo).

### Menu Pós-Login

1. **Jogar** | Usuário selecionará uma dungeon e será levado ao combate.
2. **Loja** | Exibi os itens disponíveis, usuário pode escolher o ID de um deles para comprar por uma quantidade de moedas.
3. **Inventário** | Exibi os status e itens do inventário do usuário.
4. **Modificar conta** | Exibi os dados do usuário e permite ele recomeçar o personagem, alterar o apelido e excluir conta.

## Em jogo

- Quando o usuário selecionar para começar a jogar, ele poderá escolher a dungeon desejada, depois será gerado os inimigos de forma aleatória, e levará o player direto para o combate.
- O combate funcionará por turnos, **onde o personagem pode usar 1 ação por turno**. Cada dungeon é composta por 2 camadas com 3 inimigos cada, e uma terceira camada final com o Boss da dungeon.
- As ações do player serão exibidas para seleção no começo de todo turno:

1. `ATAQUE`
2. `USAR ITEM`
3. `FUGIR`

- Quando `ATAQUE` for selecionado o player poderá selecionar entre o ataque primário e ataque especial que ele possui, depois o inimigo que o ataque será direcionado.
- Se `USAR ITEM` for selecionado, será mostrado os itens do inventário (poções), e o player poderá usar um dos itens.
- Caso `FUGIR` seja escolhido, o player terá 25% de chance de fugir do combate e voltar para o menu inicial, caso falhe perderá seu turno. <br>

- O ataque básico do jogador atinge apenas 1 alvo de sua escolha, o ataque especial causa mais dano e afeta todos os inimigos vivos, porém, possui um intervalo de 1 turno para ser usado novamente.
- Após o jogador usar seu turno, os inimigos o atacaram causando dano, no sistema o jogador sempre faz a ação antes dos inimigos.
- Quando todos os inimigos da camada forem derrotados, o combate será encerrado e o player prosseguirá para próxima camada da dungeon, ou voltar para o menu caso finalize a dungeon.

## Recursos e Recompensas

- Cada inimigo derrotado dará uma quantidade de XP, que será somado ao nível do player no fim da camada.
- Quando a dungeon for concluída, será recompensada uma quantidade de Moedas para serem usadas na loja.
- Quando o player atingir marcos de nível (5,10,15,...) ele receberá novos ataques.

## Estrutura do sistema
- `terminal-rpg(.exe):` Arquivo executável que roda o programa.
- `terminalrpg.h:` Arquivo de cabeçalho onde são declaradas as funções, structs/enums e bibliotecas usadas pelos arquivos `.c`. <br>
### Pasta src
- `main.c:` Arquivo que contém a lógica de execução do programa. <br>
- `menu.c:` Arquivo que contém as funções relacionadas aos menus. <br>
- `arquivo.c:` Arquivo que contém as funções relacionadas a manipulação de arquivos. <br>
- `usuario.c:` Arquivo que contém as funções de manipulação e criação de usuários. <br>
- `jogo.c:` Arquivo que contém as funções da jogabilidade e combate. <br>
- `functions.c:` Arquivo que possui funções gerais executadas pelos arquivos do programa. <br>

## Arquivos de Dados
### Pasta assets
- `dados-usuarios.bin:` Arquivo em binário onde são salvos os dados dos usuários. <br>
- `dungeons.txt:` Arquivo de texto onde as informações das dungeons são salvas. <br>
- `inimigos.txt:` Arquivo de texto onde as informações dos inimigos das dungeons são salvas. <br>
- `habilidades.txt:` Arquivo de texto onde as informações das habilidades existentes são salvas. <br>
- `itens.txt:` Arquivo de texto onde as informações dos itens da loja são salvas. <br>

## Compilação e Execução
*É necessário a instalação do [GCC](https://gcc.gnu.org/) no seu dispositivo para fazer a compilação.* <br>

Com o GCC instalado, copie e cole essa linha no seu terminal: <br>
`gcc src/*.c -o terminal-rpg`

Agora que o executável foi criado, cole essa linha no terminal para executá-lo: <br>
`./terminal-rpg`

## Guia de uso
*Nota: Evite interromper o sistema usando `Ctrl+C`, pois pode ocorrer perda de dados dos usuários. Use as opções de saída (Digitar 0), fornecidas nos menus.*

- Com o sistema executando no seu terminal, o menu incial já será exibido com as operações de login e registro. <br>
- No primeiro acesso, o passo inicial é o registro, seguido pelo login, assim você terá acesso ao menu principal de operações onde todas serão listadas para serem executadas de acordo com o número indicado. <br>
- Os arquivos de texto e binários de armazenamento de dados serão criados automaticamente na inicialização do sistema. <br>

## Integrantes
- Allan Donetti Calen R.A.: 22.124.002-1
- Rodrigo Akira Rosado Yatate R.A.: 22.124.093-0
