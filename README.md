*This project has been created as part of the 42 curriculum by svaladar.*

<p align="center">
  <img src="https://raw.githubusercontent.com/ayogun/42-project-badges/main/badges/philosophersm.png" alt="philosophers badge"/>
</p>

# Philosophers

## Description

**Philosophers** is a simulation of the classic [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem), formulated by Edsger Dijkstra. It is an introduction to concurrent programming with **POSIX threads** and **mutexes**.

A number of philosophers sit around a round table with a bowl of spaghetti in the middle. There is one fork between each pair of neighbours, and a philosopher needs **both** forks (left and right) to eat. Each philosopher repeats three states: **eating**, **sleeping** and **thinking**. If a philosopher goes `time_to_die` milliseconds without starting a meal, they **die** and the simulation stops.

The goal is a simulation with no data races, no deadlocks, and a death reported within 10 ms.

### How it works

- **One thread per philosopher**, plus a **monitor thread** that checks deaths and the meal count.
- **One mutex per fork.** The philosophers form a circular doubly linked list, and each one points to its left and right fork.
- **No deadlock:** even philosophers take the left fork first and odd ones take the right fork first, so they can never all wait in a circle.
- **Fair start:** even philosophers wait half of `time_to_eat` before their first meal, so neighbours do not fight for the same forks. Without this, with many philosophers (`200 800 200 200`), one of them could starve.
- **Shared state is protected:** separate mutexes guard the output (`write_lock`), the last meal time and meal count (`meal_lock`), and the stop flag (`dead`). Every message is printed under `write_lock` after checking the stop flag, so nothing is printed after a death.
- **Precise timing:** `usleep` is called in small steps of 500 µs while comparing with `gettimeofday`, so eating and sleeping last the right time and stop early if the simulation ends.
- **One philosopher:** there is only one fork, so the philosopher takes it, waits `time_to_die` and dies.

### Structure

```
philo/
├── include/philo.h
├── src/
│   ├── main.c              # setup, start of threads, join and cleanup
│   ├── parsing/
│   │   ├── parser.c        # argument validation
│   │   └── error.c         # error messages
│   ├── setup/
│   │   ├── init.c          # rules, forks and philosophers
│   │   ├── list.c          # circular doubly linked list
│   │   └── thread.c        # fork assignment and thread creation
│   ├── simulation/
│   │   ├── routine.c       # philosopher loop: eat, sleep, think
│   │   ├── forks.c         # taking and releasing forks
│   │   ├── action.c        # eating, sleeping, thinking
│   │   ├── monitor.c       # death and meal checks
│   │   └── one_philo.c     # single-philosopher case
│   └── utils/
│       ├── log.c           # state messages
│       ├── time.c          # time in ms and precise sleep
│       ├── memory.c        # cleanup
│       └── utils.c         # atoi, stop flag, start barrier
└── Makefile                # objects are built in obj/
```

## Instructions

### Compilation

```bash
cd philo
make          # builds philo
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # rebuilds everything
```

### Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
| :-- | :-- |
| `number_of_philosophers` | Number of philosophers (and forks) at the table |
| `time_to_die` | Milliseconds a philosopher can go without starting a meal |
| `time_to_eat` | Milliseconds a philosopher spends eating |
| `time_to_sleep` | Milliseconds a philosopher spends sleeping |
| `number_of_times_each_philosopher_must_eat` | *(optional)* The simulation stops when every philosopher has eaten at least this many times |

All arguments must be positive integers that fit in an `int`.

```bash
./philo 5 800 200 200       # nobody should die
./philo 5 800 200 200 7     # stops after every philosopher has eaten 7 times
./philo 4 410 200 200       # nobody should die
./philo 4 310 200 100       # a philosopher dies
./philo 1 800 200 200       # a single philosopher always dies
```

### Output

Each state change is printed on its own line, in the format required by the subject: the timestamp in milliseconds since the start, then the philosopher number.

```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
200 2 has taken a fork
...
800 1 died
```

Nothing is printed after a death.

## Resources

- [Dining Philosophers Problem – Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming – Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- `man 3 pthread_create`, `man 3 pthread_mutex_lock`, `man 2 gettimeofday`, `man 3 usleep`
- [The Little Book of Semaphores – Allen B. Downey](https://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf), chapter 4 covers the dining philosophers
- [CodeVault – Threads playlist](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)

### AI usage

AI was used to draft this README from the existing source code. The implementation was written by the author.
