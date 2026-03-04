/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:24 by wedos-sa          #+#    #+#             */
/*   Updated: 2026/03/04 17:14:23 by sofia            ###   ########.fr       */
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

typedef struct s_mutex
{
	pthread_mutex_t	*hashi;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	max_meal_lock;
	pthread_mutex_t	dead;
}	t_mutex;

typedef struct s_rules
{
	int			final_meal;
	int			dead;
	int			ph_quantity;
	int			max_meals;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		start_time;
	long		real_time;
	pthread_t	monitor;
}	t_rules;

typedef struct s_node
{
	t_mutex			*mutex;
	pthread_t		thread_id;
	t_rules			*rules;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	int				number;
	long			last_meal;
	int				meals;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_main
{
	t_rules	rules;
	t_node	*nodes;
	int		first;
	long	start;
	t_node	*temp;
}	t_main;

typedef struct s_p
{
	int		philosopher_index;
	int		hashi_index;
	t_mutex	*mutex;
}	t_p;

typedef struct s_free
{
	t_node	*current;
	t_node	*to_free;
	t_node	*start;
	int		i;
	int		total_nodes;
}	t_free;

/* ─── error.c ────────────────────────────────────────────────────────────── */

int		print_input_error(int option);
int		print_init_error(int option);

/* ─── main.c ─────────────────────────────────────────────────────────────── */

long	get_time(void);
void	p_sleep(t_node *ptr, int flag);
void	join_all_threads(t_node **nodes);
void	destroy_mutexes(t_node *nodes);

/* ─── parser.c ───────────────────────────────────────────────────────────── */

int		init_check(int argc, char **argv);
int		init_check(int argc, char **argv);
int		valid_input(char **argv);

/* ─── init.c ─────────────────────────────────────────────────────────────── */

int		init_philo(t_rules *rules, char **argv, t_node **nodes);
int		init_philo(t_rules *rules, char **argv, t_node **nodes);
void	init_main(t_node *nodes);
void	init_timers(t_main *p);
void	threads_and_mutexes(t_node **nodes);

/* ─── list.c ─────────────────────────────────────────────────────────────── */

t_node	*create_elem(int philosopher_number, t_rules *rules, t_mutex *mutex);
t_node	*list_last(t_node *head);
void	append_item(t_node **ptr, int ph_nb, t_rules *rules, t_mutex *mutex);
int		list_size(t_node *begin_list);

/* ─── routine.c ──────────────────────────────────────────────────────────── */

void	*routine(void *ptr);
void	routine_while(t_node *node);
void	eating(t_node *ptr);
void	sleeping(t_node *ptr);
void	thinking(t_node *ptr);

/* ─── one_filo.c ─────────────────────────────────────────────────────────── */

void	one_philosopher(t_node *node);

/* ─── threads.c ──────────────────────────────────────────────────────────── */

void	threads_and_mutexes(t_node **nodes);

/* ─── forks.c ────────────────────────────────────────────────────────────── */

int		take_fork(t_node *ptr);
void	put_fork(t_node *ptr);
int		take_right_fork(t_node *ptr);
int		take_left_fork(t_node *ptr);

/* ─── log.c ────────────────────────────────────────────────────────────── */

void	print_take_fork(t_node *ptr);
void	print_eating(t_node *ptr);
void	print_sleeping(t_node *ptr);
void	print_thinking(t_node *ptr);
void	print_dead(t_node *ptr);

/* ─── monitor.c ──────────────────────────────────────────────────────────── */

void	*monitor(void *head);
void	*monitor_looping(t_node *ptr, t_node *begin_list);
void	eat_monitor(t_node *ptr);
void	philo_eat_print(t_node *ptr, t_node *temp);

/* ─── utils.c ────────────────────────────────────────────────────────────── */

int		ft_atoi(const char *string);
void	free_list(t_node **begin_list);
int		is_dead(t_node *ptr);
void	wait_start(t_node *node);

#endif