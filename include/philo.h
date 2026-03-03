/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:32:32 by svaladar          #+#    #+#             */
/*   Updated: 2026/03/03 19:10:10 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

typedef enum e_status
{
	THINKING,
	EATING,
	SLEEPING,
	DEAD,
	FORK_TAKEN
}	t_status;

typedef enum e_error
{
	ERR_MALLOC,
	ERR_MUTEX,
	ERR_THREAD,
	ERR_ARGS,
	ERR_TIME,
	ERR_NUM_PHILO,
	ERR_NUM,
	ERR_MS_DIE,
	ERR_MS_EAT,
	ERR_MS_SLEEP,
	ERR_MS_MUST
}	t_error;

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}	t_fork;

typedef struct s_data
{
	int				num_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				num_must_eat;
	long			start_time;
	t_bool			dead;
	t_fork			*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	t_status		status;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
}	t_philo;

/* error.c */
int		print_input_error(int option);
int		print_init_error(int option);
int		print_time_error(int option);

/* validate.c */
int		parse_int_arg(char *arg, int *value, int err_code);
int		parse_long_arg(char *arg, long *value, int err_code);

/* input.c */
int		parse_arguments(int argc, char **argv, t_data *data);

/* utils.c */
int		ft_atoi(const char *str);
long	ft_unsigned_atol(const char *str);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);
int		ft_isdigit(int c);
int		ft_isspace(int c);
t_bool	is_simulation_over(t_philo *philo);
void	ft_usleep(long milliseconds);

/* utils_time.c */
long	get_time(void);

/* init.c */
t_bool	init_data(t_data *data, t_philo **philos);

/* cleanup.c */
void	cleanup(t_data *data, t_philo *philos);

/* log.c */
void	print_status(t_philo *philo, t_status status);

/* action.c */
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

/* routine.c */
void	*philosopher_routine(void *arg);

/* one_philo.c */
void	*one_philo_routine(void *arg);

/* monitor.c */
void	*monitor_routine(void *arg);

/* simulation.c */
t_bool	start_simulation(t_data *data, t_philo *philos);

#endif