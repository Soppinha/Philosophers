# Philosophers — Explicação Detalhada das Funções

## O problema

O Dining Philosophers é um problema clássico de concorrência: N filósofos sentam
em volta de uma mesa circular. Entre cada par de filósofos há um garfo. Para
comer, um filósofo precisa dos **dois** garfos (esquerdo e direito). Se todos
pegarem o garfo da esquerda ao mesmo tempo, ninguém consegue o da direita —
deadlock. Se um filósofo ficar sem comer por `time_to_die` milissegundos, ele morre.

Nessa implementação:
- Cada filósofo é uma **thread** (`pthread_t`)
- Cada garfo é um **mutex** (`pthread_mutex_t`)
- Um **monitor** é uma thread separada que vigia mortes e o critério de refeições

---

## Estruturas de dados

### `t_philo` — o filósofo
```
número | thread | garfo esquerdo | garfo direito | último_meal | refeições | próximo | anterior
```
A lista é **circular duplamente encadeada** — o último filósofo aponta de volta
para o primeiro. Isso permite percorrer todos os filósofos em loop sem precisar
saber onde começa ou termina, o que é útil no monitor que roda indefinidamente.

Todos os filósofos apontam para o **mesmo** `t_config` (regras) e o **mesmo**
`t_locks` (mutexes). Isso é intencional: as regras são compartilhadas e os
mutexes precisam ser os mesmos objetos para proteger os dados corretamente.

### `t_locks` — os mutexes globais
- `fork[]` — array de mutexes, um por garfo
- `write_lock` — serializa todos os `printf` para que as linhas não se misturem
- `meal_lock` — protege `last_meal` e `meals` de cada filósofo
- `time_lock` — reservado para acessos ao start_time
- `max_meal_lock` — protege a verificação de "todos comeram o suficiente"
- `dead` — protege a flag `rules->dead`

### `t_config` — as regras da simulação
Contém os tempos (`time_to_die`, `time_to_eat`, `time_to_sleep`), o número de
filósofos, max de refeições, o timestamp de início e a flag `dead`.
A flag `dead` é o mecanismo central de parada — quando vira 1, todas as threads
param no próximo `is_dead()` que chamarem.

---

## main.c

### `main`

```c
int main(int argc, char **argv)
```

Orquestra a simulação em etapas bem definidas:

1. `init_check` — valida os argumentos antes de alocar qualquer coisa
2. `init_philo` — aloca todos os filósofos e os mutexes de garfo
3. `init_simu` — inicializa os mutexes globais (write, meal, dead, etc.)
4. `init_timers` — registra o tempo de início e zera `last_meal` de todos
5. `threads_and_mutexes` — atribui os garfos e cria as threads
6. Para 1 filósofo: apenas `pthread_join` na thread única
7. Para N > 1: cria o monitor, espera o monitor terminar (`pthread_join` no monitor), depois junta todas as threads dos filósofos

A ordem importa muito. Por exemplo, `init_timers` deve rodar **antes** de
`threads_and_mutexes` porque assim que a thread nasce ela já lê `last_meal`.
Se `last_meal` fosse 0 nesse momento, o monitor calcularia `get_time() - 0`
que seria um número enorme e declararia o filósofo morto imediatamente.

O `usleep(1000)` antes de `destroy_mutexes` é um buffer de segurança: o monitor
pode ter terminado mas alguma thread de filósofo pode estar no meio de um
`pthread_mutex_unlock`. Destruir um mutex enquanto alguém ainda o usa é
comportamento indefinido. 1ms é suficiente para qualquer thread finalizar
suas operações pendentes.

### `join_all_threads`

```c
void join_all_threads(t_philo **nodes)
```

Percorre a lista circular fazendo `pthread_join` em cada thread. O critério de
parada é `curr == start` (voltou ao início), não `curr == NULL`, porque a lista
é circular e nunca tem NULL no meio.

`pthread_join` bloqueia até a thread terminar. Chamado depois que o monitor já
terminou, então todas as threads de filósofo já deveriam ter visto `is_dead()`
retornar 1 e saído do loop. O join garante que todas realmente terminaram antes
de prosseguir para destruir mutexes e liberar memória — sem isso haveria race
condition no cleanup.

### `destroy_mutexes`

```c
void destroy_mutexes(t_philo *nodes)
```

Destrói os 5 mutexes globais primeiro, depois percorre o array de garfos
destruindo cada um individualmente. Não libera memória aqui — apenas desfaz a
inicialização do mutex. A liberação da memória é feita em `free_list`.

O motivo de separar destroy de free é que `pthread_mutex_destroy` precisa ser
chamado em mutexes que foram inicializados com `pthread_mutex_init`, e precisa
acontecer antes de `free` liberar a memória onde o mutex está armazenado.

---

## parser.c

### `init_check`

```c
int init_check(int argc, char **argv)
```

Primeira linha de defesa. Verifica:
- Número de argumentos: o programa aceita 4 obrigatórios + 1 opcional (`must_eat`)
- Validade de cada argumento via `valid_input`
- Número de filósofos >= 1 (0 filósofos não faz sentido)
- Se `must_eat` foi fornecido, deve ser >= 1 (0 refeições não faz sentido)

Retorna FALSE em qualquer falha, o que faz o `main` retornar 1 sem alocar nada.

### `valid_input`

```c
int valid_input(char **argv)
```

Percorre todos os argumentos (a partir de argv[1]) e para cada um verifica:
1. Não é string vazia
2. Todos os caracteres são dígitos ('0'–'9') — sem negativos, sem pontos
3. Não estoura o `int` via `is_overflow`

O projeto da 42 define que os argumentos são inteiros positivos, então qualquer
coisa fora disso é inválida. Rejeitar aqui evita comportamento indefinido no
`ft_atoi` mais tarde.

### `is_overflow`

```c
static int is_overflow(const char *s)
```

Verifica se a string numérica representa um valor maior que `INT_MAX` (2147483647).

A lógica é:
- Mais de 10 dígitos → overflow com certeza
- Menos de 10 dígitos → sem overflow com certeza
- Exatamente 10 dígitos → compara lexicograficamente com "2147483647"

A comparação lexicográfica funciona porque os números têm o mesmo comprimento:
compara dígito a dígito da esquerda para a direita. Se um dígito do input for
maior que o correspondente do máximo, é overflow. Se for menor, não é.

---

## error.c

### `print_input_error`

```c
int print_input_error(int option)
```

Usa `write(2, ...)` em vez de `fprintf(stderr, ...)` ou `printf` por uma razão
de pureza: a 42 proíbe `fprintf` nesse projeto. O fd 2 é o stderr.

O switch por enum (`ERR_ARGS`, `ERR_NUM_PHILO`, etc.) centraliza todas as
mensagens de erro em um lugar só — se precisar mudar uma mensagem, muda aqui.
Sempre chama `print_usage` ao final para mostrar como usar o programa.

Retorna `FALSE` (que é 0) para poder ser usado como `return (print_input_error(ERR_ARGS))` — limpo e sem código duplicado.

### `print_init_error`

```c
int print_init_error(int option)
```

Separado do anterior porque erros de inicialização (como falha de `malloc`)
acontecem **depois** que os argumentos já foram validados. Não faz sentido
mostrar o usage nesse contexto — o problema não é o usuário ter digitado errado.

---

## init.c

### `init_philo`

```c
int init_philo(t_config *rules, char **argv, t_philo **nodes)
```

Ponto central de inicialização. O que ela faz em ordem:

1. Zera os campos de controle do `rules` (`dead = 0`, `start_time = 0`)
2. Aloca **um único** `t_locks` — compartilhado por todos os filósofos
3. Lê o número de filósofos de argv[1]
4. Aloca e inicializa o array de mutexes de garfo (`init_forks`)
5. Lê os tempos de argv[2..5] no `rules`
6. Cria o primeiro filósofo e então vai fazendo `append_item` para os demais

Por que um único `t_locks` para todos? Porque os mutexes precisam ser os mesmos
objetos em memória. Se cada filósofo tivesse seu próprio `meal_lock`, cada um
travaria um mutex diferente e não haveria proteção real — seria como ter um
cadeado diferente para a mesma porta.

### `init_forks`

```c
static int init_forks(t_config *rules, t_init *p)
```

Aloca `ph_quantity` mutexes em um array contíguo. N filósofos = N garfos porque
a mesa é circular: filósofo 1 tem garfo 1 à esquerda e garfo 2 à direita,
filósofo 2 tem garfo 2 à esquerda e garfo 3 à direita, e assim por diante até
o filósofo N que tem garfo N à esquerda e garfo 1 à direita.

Se o malloc falhar, libera o `t_locks` antes de retornar erro — sem esse `free`
haveria memory leak.

### `init_config`

```c
static void init_config(t_config *rules, int flag, char **argv)
```

Dividida em duas passadas com `flag` para separar responsabilidades:
- **flag 1**: zera os campos de estado (`dead`, `start_time`) — feito antes de tudo
- **flag 2**: preenche os tempos dos argumentos — feito depois de alocar os garfos

O `max_meals` recebe -1 quando não fornecido. Isso é uma sentinela: o monitor
verifica `if (max_meals > 0)` para saber se deve monitorar refeições. -1 nunca
é > 0, então o critério de refeições é simplesmente ignorado.

### `init_simu`

```c
void init_simu(t_philo *nodes)
```

Inicializa os 5 mutexes globais com `pthread_mutex_init(..., NULL)` — o NULL
significa atributos padrão (mutex não recursivo, não compartilhado entre processos).

Esses mutexes protegem dados diferentes:
- `write_lock`: garante que printf's de threads diferentes não se intercalem
- `meal_lock`: protege `last_meal` e `meals` (escritos pela thread do filósofo, lidos pelo monitor)
- `max_meal_lock`: serializa a verificação "todos comeram o suficiente"
- `dead`: protege `rules->dead` (lida e escrita por várias threads)
- `time_lock`: protege acessos ao timestamp de início

### `init_timers`

```c
void init_timers(t_simu *p)
```

Registra `get_time()` como ponto zero da simulação em `start_time` e `real_time`,
e inicializa `last_meal = start_time` e `meals = 0` para cada filósofo.

Por que isso é feito **antes** de criar as threads?

Quando uma thread nasce e entra em `routine`, o monitor já pode estar rodando.
O monitor verifica `get_time() - last_meal > time_to_die`. Se `last_meal` fosse 0
(valor padrão de um long não inicializado), a diferença seria enorme e o monitor
declararia todos os filósofos mortos imediatamente.

Inicializar `last_meal = start_time` garante que o "relógio pessoal" de cada
filósofo começa no mesmo momento que a simulação.

---

## list.c

### `create_elem`

```c
t_philo *create_elem(int philosopher_number, t_config *rules, t_locks *mutex)
```

Aloca um nó `t_philo` e o inicializa. Os ponteiros `next` e `prev` começam como
NULL — a lista circular só se fecha quando `append_item` conecta o último ao primeiro.

Todos os filósofos recebem ponteiros para o **mesmo** `rules` e o **mesmo**
`mutex`. Não há cópias — qualquer alteração em `rules->dead` feita pelo monitor
é imediatamente visível para todos os filósofos.

### `append_item`

```c
void append_item(t_philo **ptr, int ph_nb, t_config *rules, t_locks *mutex)
```

Adiciona um novo filósofo ao final da lista e atualiza os 4 ponteiros envolvidos:

```
Antes:  head <-> ... <-> last -> head
Depois: head <-> ... <-> last <-> item -> head
                              head->prev = item
```

Por que lista circular e não linear? Porque os filósofos estão numa mesa
circular — o filósofo N compartilha um garfo com o filósofo 1. A estrutura
circular torna isso natural: `nodes->prev` é sempre o filósofo à esquerda.
Também facilita o monitor, que pode percorrer indefinidamente sem lógica de
"voltei ao início".

### `list_last`

```c
t_philo *list_last(t_philo *head)
```

Retorna o último nó da lista. O critério `node->next != head && node->next != NULL`
funciona em dois cenários:
- Durante a construção (lista ainda não é circular): para quando `next == NULL`
- Após a construção (lista circular): para quando `next == head` (voltou ao início)

### `list_size`

```c
int list_size(t_philo *begin_list)
```

Conta quantos nós há na lista. Percorre até encontrar NULL (lista ainda em
construção) ou usar em contextos onde o tamanho precisa ser calculado
dinamicamente em vez de usar `rules->ph_quantity`.

---

## thread.c

### `threads_and_mutexes`

```c
void threads_and_mutexes(t_philo **nodes)
```

Percorre a lista e para cada filósofo chama `assign_forks_and_spawn`. O índice
`index_mutex` avança junto — filósofo 1 recebe garfo[0] à esquerda, filósofo 2
recebe garfo[1] à esquerda, e assim por diante.

Por que não criar as threads antes de atribuir todos os garfos? Porque a thread
nasce e já pode tentar pegar o garfo. Se os garfos não tivessem sido atribuídos
antes, a thread leria ponteiros NULL e haveria segfault. A função faz a
atribuição e cria a thread no mesmo passo via `assign_forks_and_spawn`.

### `assign_forks_and_spawn`

```c
static void assign_forks_and_spawn(t_philo *node, t_philo *begin_list,
                                    pthread_mutex_t *fork, int index_mutex)
```

Atribui `left` e `right` de cada filósofo:
- Filósofos intermediários: `left = fork[i]`, `right = fork[i+1]`
- Último filósofo: `left = fork[N-1]`, `right = fork[0]` (o garfo do primeiro)

Essa atribuição do último filósofo fecha o círculo de garfos. Sem ela, o último
filósofo teria `right = NULL` e o acesso seria inválido.

Depois de atribuir os garfos, chama `pthread_create` com `routine` como função
da thread. O terceiro argumento NULL usa atributos padrão da thread.

---

## routine.c

### `routine`

```c
void *routine(void *ptr)
```

Função que cada thread de filósofo executa. Recebe `void *` porque é assim que
`pthread_create` passa argumentos — o cast para `t_philo *` é imediato.

Caso especial de 1 filósofo é tratado aqui e não no `main` porque o `main` já
criou a thread — quando `routine` roda, já está dentro da thread. `one_philosopher`
lida com a lógica determinística do filósofo solitário.

`wait_start` garante sincronização: todas as threads ficam em busy-wait até
`start_time != 0`, que é setado em `init_timers`. Isso faz todas as threads
começarem quase simultaneamente, evitando que threads criadas por último comecem
"atrasadas" em relação ao seu `time_to_die`.

### `routine_while`

```c
void routine_while(t_philo *node)
```

Loop principal: comer → dormir → pensar → repetir.

Verifica `is_dead` entre cada ação para que a thread pare o mais rápido possível
após uma morte. Sem essas verificações, uma thread poderia continuar imprimindo
ações de sono e pensamento mesmo depois que outro filósofo morreu, o que violaria
o comportamento esperado.

O `break` no `do_eat_cycle` captura tanto a morte detectada ao tentar pegar
garfos quanto casos onde a thread simplesmente não conseguiu o mutex.

### `do_eat_cycle`

```c
static int do_eat_cycle(t_philo *node)
```

Sequência completa de um ciclo de alimentação:

1. `take_fork` — pega os dois garfos (pode bloquear esperando o mutex)
2. Verifica `is_dead` — pode ter morrido enquanto esperava o garfo
3. Pega `write_lock` — serializa os prints
4. Verifica `is_dead` novamente — pode ter morrido entre pegar o garfo e o write_lock
5. Imprime dois "has taken a fork" (um para cada garfo)
6. Solta o `write_lock`
7. `eating` — atualiza `last_meal` e dorme pelo `time_to_eat`
8. `put_fork` — solta os dois garfos

As verificações duplas de `is_dead` dentro da posse dos mutexes são necessárias
porque o monitor pode ter setado `dead = 1` entre qualquer duas operações.
Imprimir após uma morte seria uma violação do subject da 42.

---

## action.c

### `eating`

```c
void eating(t_philo *ptr)
```

Atualiza `last_meal` e `meals` **antes** de imprimir e **antes** de dormir.

Por que atualizar antes do print? O monitor lê `last_meal` a qualquer momento.
Se atualizarmos `last_meal` depois do print, existe uma janela de tempo entre
"começou a comer" e "registrou que comeu" onde o monitor poderia calcular uma
diferença de tempo grande e declarar uma morte falsa.

A sequência é:
```
1. meal_lock → atualiza last_meal e meals → unlock  ← monitor vai ver isso
2. is_dead check
3. write_lock → print "is eating" → unlock
4. p_sleep (dorme time_to_eat)
```

### `sleeping`

```c
void sleeping(t_philo *ptr)
```

Imprime "is sleeping" sob `write_lock` e chama `p_sleep` com flag 2. O
`is_dead` antes do print evita imprimir se a simulação já acabou — isso
aconteceria se o monitor declarou morte enquanto o filósofo estava comendo.

### `thinking`

```c
void thinking(t_philo *ptr)
```

Para número **ímpar** de filósofos, adiciona um delay inteligente. O problema
com número ímpar é que há sempre um filósofo a mais do que garfos disponíveis
em um dado momento — um deles inevitavelmente vai ter que esperar. Sem delay,
ele fica tentando pegar garfos imediatamente e pode causar starvation.

O delay calculado é:
```
delay = time_to_die - time_to_eat - time_to_sleep
```
Se esse delay for maior que `time_to_eat`, aguarda metade dele (`usleep(delay * 500)` — que são `delay/2` milissegundos).

Para número **par** de filósofos, a alternância natural par/ímpar já distribui
os garfos uniformemente e não é necessário delay extra.

---

## forks.c

### `take_fork`

```c
int take_fork(t_philo *ptr)
```

O coração da solução de deadlock. Filósofos **pares** pegam o garfo esquerdo
primeiro; filósofos **ímpares** pegam o direito primeiro.

Por que isso previne deadlock? Imagine 4 filósofos sem essa lógica: todos
pegam o garfo esquerdo → todos bloqueiam esperando o direito → deadlock.

Com a inversão, filósofo 1 (ímpar) pega direito primeiro, filósofo 2 (par) pega
esquerdo primeiro. Eles competem pelo mesmo garfo mas de lados opostos, o que
garante que pelo menos um par de filósofos sempre consegue os dois garfos.

### `take_right_fork` e `take_left_fork`

```c
int take_right_fork(t_philo *ptr)
int take_left_fork(t_philo *ptr)
```

Ambas seguem o mesmo padrão:
```
lock(primeiro_garfo)
if is_dead → unlock(primeiro_garfo) → return FALSE
lock(segundo_garfo)
if is_dead → put_fork (solta os dois) → return FALSE
return TRUE
```

A verificação de `is_dead` após cada lock é crucial. O monitor pode ter setado
`dead = 1` enquanto a thread estava bloqueada esperando o mutex. Se não
verificarmos aqui, a thread continuaria comendo mesmo após uma morte declarada,
o que geraria prints inválidos.

A diferença entre as duas funções é apenas a ordem: `take_right` trava right
depois left, `take_left` trava left depois right. Essa inversão simétrica é o
que quebra a possibilidade de deadlock circular.

### `put_fork`

```c
void put_fork(t_philo *ptr)
```

Libera os dois garfos. Não verifica `is_dead` — os garfos devem ser soltos
independentemente do estado da simulação. Manter mutexes travados após o fim
da simulação causaria que `destroy_mutexes` no main travasse tentando destruir
um mutex ainda bloqueado.

---

## time.c

### `get_time`

```c
long get_time(void)
```

Retorna o tempo atual em milissegundos usando `gettimeofday`. A conversão é:
```
milissegundos = segundos × 1000 + microssegundos ÷ 1000
```

Por que milissegundos? O subject da 42 define todos os tempos em ms. Usar ms
evita conversões espalhadas pelo código.

`gettimeofday` é obsoleto pelo POSIX moderno (que prefere `clock_gettime`), mas
é explicitamente permitido pelo subject da 42 e tem resolução suficiente para
o problema.

### `p_sleep`

```c
void p_sleep(t_philo *ptr, int flag)
```

Implementa um sleep **interrompível**. Em vez de um único `usleep(time_to_eat * 1000)`,
faz um loop de `usleep(500)` (meio milissegundo) verificando `is_dead` a cada iteração.

Por que não usar `usleep` diretamente?

Se o monitor declara morte enquanto um filósofo está dormindo num `usleep` longo,
a thread ficaria "presa" no sleep até ele terminar e só então veria `is_dead`.
Na prática, uma thread poderia continuar "existindo" por centenas de milissegundos
após a morte, o que atrasa o fim da simulação e pode causar comportamento incorreto.

O `usleep(500)` de 0.5ms é um bom equilíbrio: baixo CPU overhead (não é busy-wait
puro) e resposta rápida a mortes.

---

## monitor.c

### `monitor`

```c
void *monitor(void *head)
```

Thread separada que observa a simulação de fora. Roda em loop com `usleep(500)`
entre iterações — checa a cada meio milissegundo se alguém morreu ou se todos
comeram o suficiente.

Por que uma thread separada e não verificar dentro de cada thread de filósofo?

Porque cada thread de filósofo está "dentro do próprio personagem" — bloqueada
esperando mutex, dormindo, comendo. Centralizar a lógica de morte em um monitor
externo separa responsabilidades: threads fazem as ações, monitor faz a vigilância.

No início do loop, reseta `dead = 0` (a flag pode ter ficado suja de uma rodada
anterior, mas na prática essa é a primeira coisa que roda).

### `monitor_looping`

```c
void *monitor_looping(t_philo *ptr, t_philo *begin_list)
```

Uma iteração completa do monitor. Sequência:

1. Se `max_meals > 0`, chama `eat_monitor` para verificar se todos comeram o suficiente
2. Se alguém morreu (detectado por `eat_monitor`), retorna NULL para parar o loop
3. Percorre todos os filósofos verificando morte individual via `check_death`

Retorna `NULL` quando a simulação deve parar (morte detectada), `(void *)1`
quando tudo está normal e o loop deve continuar.

### `check_death`

```c
static int check_death(t_philo *ptr)
```

Para cada filósofo, verifica se `get_time() - last_meal > time_to_die`.

Por que ler `last_meal` sob `meal_lock`?

A thread do filósofo escreve em `last_meal` dentro de `eating` (também sob
`meal_lock`). Sem o lock, o monitor poderia ler um valor parcialmente atualizado
de `last_meal` (race condition), resultando em detecção de morte falsa ou
atrasada.

Quando detecta morte:
1. Ainda sob `meal_lock`, trava `dead` mutex e seta `dead = 1`
2. Solta `meal_lock` — importante soltar antes de tentar `write_lock`
3. Trava `write_lock` e imprime a morte
4. Solta `write_lock`

A ordem de locks (meal → dead → solta meal → write) evita deadlock. Se
tentasse pegar `write_lock` enquanto ainda segura `meal_lock`, poderia entrar
em deadlock com uma thread de filósofo que faz o caminho inverso.

### `eat_monitor`

```c
void eat_monitor(t_philo *begin_list)
```

Verifica se **todos** os filósofos atingiram `max_meals`. A lógica é conservadora:
percorre todos; se qualquer um ainda não atingiu, retorna imediatamente sem fazer
nada. Só prossegue (imprime sumário e seta `dead = 1`) se todos completaram.

Cada `meals` é lido sob `meal_lock` para consistência com as escritas em `eating`.

### `philo_eat_print`

```c
void philo_eat_print(t_philo *ptr, t_philo *temp)
```

Imprime o sumário final de refeições de todos os filósofos. Todo o print é feito
sob `write_lock` para não intercalar com possíveis prints de outras threads que
ainda não viram `dead = 1`.

---

## log.c

Todas as funções calculam o timestamp como `get_time() - rules->real_time`,
que dá o tempo **relativo ao início da simulação** em milissegundos. Isso é
obrigatório pelo subject da 42 — o tempo absoluto do sistema seria irrelevante
para análise da simulação.

### `print_dead`

```c
void print_dead(t_philo *ptr)
```

Usa escape ANSI `\033[31m` para vermelho e `\033[0m` para resetar. Não é
obrigatório pelo subject mas facilita muito identificar a morte visualmente
no terminal. O `%ld` (long) é usado porque o timestamp pode ser grande.

---

## utils.c

### `ft_atoi`

```c
int ft_atoi(const char *string)
```

Reimplementação do `atoi` da libc. A 42 proíbe usar funções não autorizadas,
então cada função de biblioteca precisa ser reimplementada quando necessária.

Lógica:
1. Pula espaços em branco (ASCII 9–13 e 32)
2. Detecta sinal (`+` ou `-`)
3. Converte dígitos para número (`nb = nb * 10 + (c - '0')`)

O tipo interno `unsigned long int` para `nb` previne overflow durante a
conversão (antes do sinal ser aplicado). O resultado é multiplicado pelo `sign`
no final.

### `is_dead`

```c
int is_dead(t_philo *ptr)
```

Leitura thread-safe da flag `dead`. Trava `dead` mutex, lê o valor, destranca
e retorna.

Por que não ler `rules->dead` diretamente?

Sem o mutex, leituras e escritas concorrentes em `rules->dead` constituem uma
**data race** — comportamento indefinido em C. Em arquiteturas com cache de CPU
multinível, uma thread pode nem ver a escrita de outra sem uma barreira de
memória, que o mutex fornece.

Essa função é chamada em absolutamente todo lugar — antes de cada ação, dentro
de `p_sleep`, no loop do monitor. É a principal forma de comunicação entre o
monitor e as threads.

### `wait_start`

```c
void wait_start(t_philo *node)
```

Busy-wait leve: loop com `usleep(50)` até `start_time != 0`.

`start_time` é setado em `init_timers`, que roda **antes** de `threads_and_mutexes`.
Mas há uma corrida potencial: `init_timers` seta `start_time` na `t_config` que é
compartilhada. Quando a thread começa a executar, ela lê `start_time`.

Na prática, como `init_timers` roda antes de criar qualquer thread, `start_time`
já será != 0 quando a thread iniciar. Mas `wait_start` existe como salvaguarda
para threads que possam começar antes do scheduler chegar no `init_timers`
em edge cases de CPU muito lento. O `usleep(50)` reduz a pressão de CPU
do busy-wait.

---

## one_philo.c

### `one_philosopher`

```c
void one_philosopher(t_philo *node)
```

Caso especial e determinístico: com 1 filósofo e 1 garfo, é impossível comer
(precisaria de 2 garfos). O filósofo vai inevitavelmente morrer.

A função:
1. Trava o único garfo (o `left` do filósofo)
2. Imprime "has taken a fork"
3. Faz `usleep(time_to_die * 1000)` — espera exatamente o tempo até a morte
4. Imprime "died"
5. Solta o garfo

Por que `usleep` direto e não `p_sleep`? Porque o filósofo solitário não tem
monitor — o `main` faz apenas `pthread_join` na thread única sem criar monitor.
Não há race condition aqui, então o sleep interrompível é desnecessário.

O garfo é travado durante todo o processo para simular que o filósofo "tentou
comer mas não conseguia o segundo garfo" — semanticamente correto com o problema
dos filósofos.

---

## memory.c

### `free_list`

```c
void free_list(t_philo **begin_list)
```

Libera toda a memória alocada pela simulação em ordem segura:

1. `free_mutex` — libera o array de garfos e o struct `t_locks`
2. `free_nodes(start->next, total, 1)` — libera todos os nós exceto o primeiro
3. `free(start)` — libera o primeiro nó (que continha o ponteiro para os mutexes)
4. `*begin_list = NULL` — zera o ponteiro para prevenir double-free

A ordem importa porque o `t_locks` é acessado via `start->mutex`. Se liberarmos
`start` antes de `free_mutex`, perdemos o ponteiro para o `t_locks`.

Liberar o primeiro nó por último (não no loop de `free_nodes`) é uma escolha
deliberada para manter acesso ao `rules->ph_quantity` (necessário para saber
quantos nós liberar) até o final.

### `free_mutex`

```c
static void free_mutex(t_philo *start)
```

Libera o array `fork` e depois o próprio struct `t_locks`. As verificações de
NULL (`if (!start->mutex)`, `if (start->mutex->fork)`) previnem double-free caso
algo tenha falhado durante a inicialização antes de completar a alocação.

### `free_nodes`

```c
static void free_nodes(t_philo *current, int total, int start_i)
```

Libera `total - start_i` nós consecutivos. O `start_i = 1` significa que começa
do segundo nó — o primeiro é liberado separadamente em `free_list`.

O padrão clássico de liberação de lista encadeada:
```
to_free = current;       // salva referência ao nó atual
current = current->next; // avança antes de liberar
free(to_free);           // libera o nó salvo
```
Sem isso, acessar `current->next` depois de `free(current)` seria use-after-free.
