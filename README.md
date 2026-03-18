*This project has been created as part of the 42 curriculum by svaladar.*

# 🍔🍴 Philosophers

## Description

**Philosophers** is a simulation of the classic [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem), originally formulated by Edsger Dijkstra. It is a foundational exercise in concurrent programming and operating systems design.

In this simulation, a number of philosophers sit around a circular table with a bowl of spaghetti in the center. Between each pair of adjacent philosophers lies a single fork. A philosopher needs **both** the fork to their left and the fork to their right to eat. They spend their time alternating between three states: **thinking**, **eating**, and **sleeping**. If a philosopher goes too long without eating, they **die** — and the simulation ends.

The goal of this project is to implement a correct, race-condition-free, and deadlock-free solution using **POSIX threads** and **mutexes**.

### Key features

- Each philosopher runs in its own thread.
- Each fork is protected by a mutex to prevent data races.
- A dedicated monitor thread watches for deaths and meal completion.
- Odd/even philosopher ordering is used to avoid deadlock when acquiring forks.
- A special case handles the single-philosopher scenario correctly.
- Optional argument: minimum number of meals each philosopher must eat before the simulation ends successfully.

---

## Instructions

### Compilation

```bash
make
```

This produces the `philo` binary in the current directory.

### Usage

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) at the table |
| `time_to_die` | Time in milliseconds before a philosopher dies without eating |
| `time_to_eat` | Time in milliseconds a philosopher spends eating |
| `time_to_sleep` | Time in milliseconds a philosopher spends sleeping |
| `number_of_times_each_philosopher_must_eat` | *(optional)* Simulation ends when all philosophers have eaten at least this many times |

All time values are in **milliseconds**. All arguments must be positive integers.

### Examples

```bash
# 5 philosophers, die after 800ms, eat for 200ms, sleep for 200ms
./philo 5 800 200 200

# Same, but stops after each philosopher has eaten at least 7 times
./philo 5 800 200 200 7

# Edge case: single philosopher (will always die)
./philo 1 800 200 200
```

### Cleanup

```bash
make clean    # Remove object files
make fclean   # Remove object files and binary
make re       # Full rebuild
```

---

## Resources

### Topic references

- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [pthread_mutex_lock(3) — Linux man page](https://linux.die.net/man/3/pthread_mutex_lock)
- [gettimeofday(2) — Linux man page](https://linux.die.net/man/2/gettimeofday)
- [The Little Book of Semaphores — Allen B. Downey (free PDF)](https://greenteapress.com/semaphores/LittleBookOfSemaphores.pdf) — Chapter 4 covers the dining philosophers problem in depth.
- [CodeVault — Threads playlist on YouTube](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2) — Practical C threading tutorials.

### AI usage

Claude (Anthropic) was used during this project for the following task:

- **Documentation**: Generating this README based on the project source code and 42 specification requirements.

AI was **not** used to write the core simulation logic, data structures, or threading code — those were authored directly by the student.
