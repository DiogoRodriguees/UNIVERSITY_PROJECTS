# Projeto 1

PROFESSOR:
 O professor inicia com a abertura da sala, liberando os alunos
e monitores para entrarem. Essa liberação ocorre liberando o semaforo "s_sala" e 
atribuindo TRUE para a variavel sala_aberta. Após um determindado tempo(T_SALA_ABERTA)
os professor avisa os alunos e monitores que não é possivel entrar na sala.
Esse aviso ocorre atribuindo o valor FALSE para a variavel "sala_aberta".
Em seguida ele aguarda e liberação do semaforo(s_fechar_sala) que é liberado pelo ultimo
monitor que sair da sala, garantindo que não existe mais nenhum aluno na sala.
O professor ao sair tambem libera um token para os alunos que estão presos no semaforo, uma vez que nenhum 
monitor vai conseguir entrar para liberar esse alunos.

MONITORES:
 Ao executar, o monitor verifica se pode entrar na sala. Caso seja possivel, ele verifica se
existe algum monitor que deseja sair. Caso exista, ele libera esse monitor. Caso não exista
ele libera(ALUNOS_POR_GRUPO) tokens para novos alunos entrarem. Após determinado tempo(T_MONITOR_SALA)
o monitor sinaliza que deseja sair. Essa sinalização ocorre com a atribuição "TRUE" para a variavel(monitor_deseja_sair).
Se o monitor for o ultimo a sair ele libera um token para o semaforo "s_fechar_sala", assim o professor termina sua execução
fechando a sala. Se ao sair o monitor mantiver a condição de X alunos por monitor, ele sai normalmente, caso contrario,
ele tenta pegar um token de saida no semaforo(s_saida_monitores). Esse semaforo recebe post quando um monitor entra ou 
quando um alunos sai e a condição(X alunos por monitor) é satisfeita.

ALUNOS:
 Ao entrar os alunos aguardam até que os monitores liberem tokens para entrada ou algum aluno saia. Essa liberação
ocorre pelo metodo post no semaforo "s_alunos". Ao entra o aluno verifica se a entrada de alunos na sala é possivel. 
Caso seja possivel o aluno entra e permance um tempo(T_ALUNO_SALA) antes de sair da sala. 
Ao sair o aluno verifica se é possivel liberar um monitor para que ele saia. 
Para que o monitor seja libera o aluno deve saber se a quantidade de alunos na sala é menor 
que a quantidade de ((monitores - 1) * alunos por grupo). Essa verificação garante X alunos por monitor.


FUNÇÕES EXTRAS:
 Para simular um tempo entre as chegadas dos monitores, foi criado um variavel(T_CRIACAO_MONITOR) que é
utilizada na função sleep antes que o proximo monitor seja criado.
O tempo que o professor, monitor e aluno permanecem na sala e determinado pelas vaariaveis define (T_ALUNO_SALA, 
T_MONITOR_SALA, T_PROFESSOR_NA SALA) que são passadas como parametro nas funções sleep().

## Como compilar

```bash
$ make main
```

## Como executar

```bash
$ ./main.o
```

## Exemplo

```bach
# compilando arquivo
$ make main

# executando
$ ./main.o
```
