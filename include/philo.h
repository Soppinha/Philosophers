/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:11:56 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:59:17 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

typedef enum e_error
{
	ERR_ARGS,
	ERR_NUM_PHILO,
	ERR_EMPTY,
	ERR_DIGIT,
	ERR_OVERFLOW,
	ERR_MEALS,
	ERR_MALLOC
}	t_error;

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct s_locks
{
	pthread_mutex_t	*fork;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	max_meal_lock;
	pthread_mutex_t	dead;
}	t_locks;

typedef struct s_config
{
	int			dead;
	int			ph_quantity;
	int			max_meals;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		start_time;
	long		real_time;
	pthread_t	monitor;
}	t_config;

typedef struct s_philo
{
	t_locks			*mutex;
	pthread_t		thread_id;
	t_config		*rules;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	int				number;
	long			last_meal;
	int				meals;
	struct s_philo	*next;
	struct s_philo	*prev;
}	t_philo;

typedef struct s_simu
{
	t_config	rules;
	t_philo		*nodes;
	int			first;
	long		start;
	t_philo		*temp;
}	t_simu;

typedef struct s_init
{
	int		philo_index;
	int		fork_index;
	t_locks	*mutex;
}	t_init;

/* ─── error.c ────────────────────────────────────────────────────────────── */

int		print_input_error(int option);
int		print_init_error(int option);

/* ─── main.c ─────────────────────────────────────────────────────────────── */

void	join_all_threads(t_philo **nodes);
void	destroy_mutexes(t_philo *nodes);

/* ─── time.c ─────────────────────────────────────────────────────────────── */

long	get_time(void);
void	p_sleep(t_philo *ptr, int flag);

/* ─── parser.c ───────────────────────────────────────────────────────────── */

int		init_check(int argc, char **argv);
int		valid_input(char **argv);

/* ─── init.c ─────────────────────────────────────────────────────────────── */

int		init_philo(t_config *rules, char **argv, t_philo **nodes);
void	init_simu(t_philo *nodes);
void	init_timers(t_simu *p);

/* ─── list.c ─────────────────────────────────────────────────────────────── */

t_philo	*create_elem(int philosopher_number, t_config *rules, t_locks *mutex);
t_philo	*list_last(t_philo *head);
void	append_item(t_philo **ptr, int ph_nb, t_config *rules, t_locks *mutex);
int		list_size(t_philo *begin_list);

/* ─── action.c ───────────────────────────────────────────────────────────── */

void	eating(t_philo *ptr);
void	sleeping(t_philo *ptr);
void	thinking(t_philo *ptr);

/* ─── routine.c ──────────────────────────────────────────────────────────── */

void	*routine(void *ptr);
void	routine_while(t_philo *node);

/* ─── one_philo.c ────────────────────────────────────────────────────────── */

void	one_philosopher(t_philo *node);

/* ─── thread.c ───────────────────────────────────────────────────────────── */

void	threads_and_mutexes(t_philo **nodes);

/* ─── forks.c ────────────────────────────────────────────────────────────── */

int		take_fork(t_philo *ptr);
void	put_fork(t_philo *ptr);
int		take_right_fork(t_philo *ptr);
int		take_left_fork(t_philo *ptr);

/* ─── log.c ──────────────────────────────────────────────────────────────── */

void	print_take_fork(t_philo *ptr);
void	print_eating(t_philo *ptr);
void	print_sleeping(t_philo *ptr);
void	print_thinking(t_philo *ptr);
void	print_dead(t_philo *ptr);

/* ─── monitor.c ──────────────────────────────────────────────────────────── */

void	*monitor(void *head);
void	*monitor_looping(t_philo *ptr, t_philo *begin_list);
void	eat_monitor(t_philo *begin_list);
void	philo_eat_print(t_philo *ptr, t_philo *temp);

/* ─── utils.c ────────────────────────────────────────────────────────────── */

int		ft_atoi(const char *string);
int		is_dead(t_philo *ptr);
void	wait_start(t_philo *node);

/* ─── memory.c ───────────────────────────────────────────────────────────── */

void	free_list(t_philo **begin_list);

#endif