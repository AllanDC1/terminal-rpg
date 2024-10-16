# Terminal-RPG

Projeto 2 do curso de CC 2° sem Noturno da FEI.

## Funcionalidades do usuário

### Menu inicial

- _Login_: Usuário informa login (nome de usuário) e senha.
- _Registro_: Usuário informa nome de usuário, senha e apelido dentro do jogo.

### Menu Pós-Login

- _Jogar_: Usuário começa a jogar.
- _Loja_: Exibir os itens disponíveis, usuário poderá escolher 1 deles para comprar por uma quantidade de moedas.
- _Inventário_: Exibi os itens no inventário do player.
- _Alterar conta_: Exibi os dados do usuário e permite ele excluir conta, alterar nome de usuário, etc.

## Em jogo

- Quando o usuário selecionar para começar a jogar, um número aleatório, correspondente a um índice do array das dungeons, será gerado criando o combate com os inimigos de acordo com a dungeon.
- O combate funcionará por turnos, **onde o personagem pode usar 1 ação por turno**.
- A primeira tela de combate será mostrada, apresentando o personagem e os inimigos (exemplo no `tela_combate.txt`).
- As ações do player serão exibidas para seleção:

1. `ATAQUE`
2. `ITEM`
3. `FUGIR`

- Quando `ATAQUE` for selecionado o player poderá selecionar entre o ataque primário e ataque especial que ele possui, depois o inimigo que o ataque será direcionado.
- Se `ITEM` for selecionado, será mostrado os itens do inventário, como poções, e o player poderá usar um dos itens.
- Caso `FUGIR` seja escolhido, o player terá 25% de chance de fugir do combate e voltar para o menu inicial, caso falhe perderá seu turno.

- Após cada turno a tela de combate será mostrada, alterando caso algum inimigo seja derrotado, etc.
- Quando todos os inimigos forem derrotados, o combate será encerrado ? e o player poderá prosseguir para próxima camada da dungeon, ou voltar para o menu. ?

## Recursos e Recompensas

- Cada inimigo derrotado dará uma quantidade X de XP, ? de acordo com sua dificuldade. ?
- ? O inimigo final da dungeon dará a maior quantidade de XP se derrotado. ?
- Quando a dungeon for concluída, será recompensada uma quantidade X de Moedas.
- A XP será somada no nível do player, e as Moedas poderão ser gastas na loja.
- Quando o player atingir marcos de nível (5,10,15,...) ele receberá novos ataques.

## Estrutura das mecânicas

- Os players serão structs armazenadas em binário contendo: <br>

  - Nome de usuário (login) <br>
  - Senha <br>
  - Apelido (nickname dentro do jogo) <br>
  - Nível (XP float) <br>
  - Inventário de consumíveis <br>
  - Ataque básico
  - Ataque especial
  - Status <br>

- Status será uma struct contendo: <br>

  - Vida <br>
  - Ataque <br>
  - Velocidade ? <br>

- Dungeons serão structs armazenadas em txt contendo os seguintes atributos: <br>

  - ID (índice no array) <br>
  - Nome <br>
  - Dificuldade <br>
  - Recompensa (Moedas e Itens) <br>

- Inimigos serão structs armazenados em txt contendo os seguintes atributos: <br>

  - ID ? <br>
  - Nome <br>
  - Status <br>
  - Recompensa (XP) <br>

- Ataques serão structs armazenados em txt contendo os seguintes atributos: <br>

  - ID <br>
  - Nome <br>
  - Dano <br>
  - Velocidade ? <br>

- A loja será armazenada em binario contendo 2 tipos de poções (vida e força) com diferentes potências. <br>

  - _Poções:_ <br>
  - ID <br>
  - Nome <br>
  - Vida recuperada <br>
  - Dano Aumentado <br>
  - Preço <br>

...
