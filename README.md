# Projeto 2-Terminal RPG (2° Sem. CC FEI)

## Objetivo do Projeto
Projeto desenvolvido na linguagem C, por alunos no 2° semestre do curso de Ciência da Computação da FEI. <br> <br>
A ideia do projeto se trata de um jogo simples de RPG, executado no terminal, onde os jogadores passarão por dungeons, 
enfrentando inimigos em combates por turno, subindo de nível e ganhando novas habilidades. <br> <br>
Além disso o sistema conta com registro e login para os usuários operarem pelo sistema, e outra funções como modificar dados da sua conta, e uma loja para comprar itens e verifica-los no inventário.

## Funcionalidades do usuário

### Menu inicial

- _Login_: Usuário informa login (nome de usuário) e senha.
- _Registro_: Usuário informa nome de usuário, senha e apelido (mostrado dentro do jogo).

### Menu Pós-Login

- _Jogar_: Usuário selecionará uma dungeon e será levado ao combate.
- _Loja_: Exibi os itens disponíveis, usuário pode escolher o ID de um deles para comprar por uma quantidade de moedas.
- _Inventário_: Exibi os status e itens do inventário do usuário.
- _Modificar conta_: Exibi os dados do usuário e permite ele recomeçar personagem, alterar o apelido e excluir conta.

## Em jogo

- Quando o usuário selecionar para começar a jogar, ele poderá escolher a dungeon desejada, depois será gerado os inimigos de forma aleatória, e levará o player direto para o combate.
- O combate funcionará por turnos, **onde o personagem pode usar 1 ação por turno**. Cada dungeon é composta por 2 camadas com 3 inimigos cada, e uma terceira camada final com o Boss da dungeon.
- As ações do player serão exibidas para seleção no começo de todo turno:

1. `ATAQUE`
2. `USAR ITEM`
3. `FUGIR`

- Quando `ATAQUE` for selecionado o player poderá selecionar entre o ataque primário e ataque especial que ele possui, depois o inimigo que o ataque será direcionado.
- Se `USAR ITEM` for selecionado, será mostrado os itens do inventário (poções), e o player poderá usar um dos itens.
- Caso `FUGIR` seja escolhido, o player terá 25% de chance de fugir do combate e voltar para o menu inicial, caso falhe perderá seu turno.

- Quando todos os inimigos da camada forem derrotados, o combate será encerrado e o player prosseguirá para próxima camada da dungeon, ou voltar para o menu caso finalize a dungeon.

## Recursos e Recompensas

- Cada inimigo derrotado dará uma quantidade de XP, que será somado ao nível do player no fim da camada.
- Quando a dungeon for concluída, será recompensada uma quantidade de Moedas para serem usadas na loja.
- Quando o player atingir marcos de nível (5,10,15,...) ele receberá novos ataques.

## Estrutura do sistema

...
