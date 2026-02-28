/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:32:32 by svaladar          #+#    #+#             */
/*   Updated: 2026/02/28 18:36:46 by sopinha          ###   ########.fr       */
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

int		error_input_msg(int option);
int		error_init_msg(int option);
int		error_time_msg(int option);
int		validate_and_convert(char *arg, int *value, int err_code);
int		validate_and_convert_long(char *arg, long *value, int err_code);
int		parse_arguments(int argc, char **argv, t_data *data);
int		ft_atoi(const char *str);
long	ft_unsigned_atol(const char *str);
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(char *s, int fd);
int		ft_isdigit(int c);
int		ft_isspace(int c);
t_bool	init_data(t_data *data, t_philo **philos);
void	cleanup(t_data *data, t_philo *philos);
long	get_time(void);
t_bool	is_simulation_over(t_philo *philo);
void	ft_usleep(long milliseconds);
void	print_status(t_philo *philo, t_status status);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	*philosopher_routine(void *arg);
void	*one_philo_routine(void *arg);
void	*monitor_routine(void *arg);
t_bool	start_simulation(t_data *data, t_philo *philos);

#endif