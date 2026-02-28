/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 12:54:13 by sopinha           #+#    #+#             */
/*   Updated: 2026/02/27 05:13:38 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(t_fork) * data->num_philos);
	if (!data->forks)
		return (error_init_msg(ERR_MALLOC));
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
			return (error_init_msg(ERR_MUTEX));
		data->forks[i].id = i;
		i++;
	}
	return (TRUE);
}

static t_bool	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
		return (error_init_msg(ERR_MUTEX));
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0)
		return (error_init_msg(ERR_MUTEX));
	if (pthread_mutex_init(&data->meal_lock, NULL) != 0)
		return (error_init_msg(ERR_MUTEX));
	return (TRUE);
}

static t_bool	init_philo_entry(t_philo *philos, t_data *data, int i)
{
	philos[i].id = i + 1;
	philos[i].meals_eaten = 0;
	philos[i].last_meal_time = 0;
	philos[i].status = THINKING;
	philos[i].data = data;
	if (pthread_mutex_init(&philos[i].meal_mutex, NULL) != 0)
		return (error_init_msg(ERR_MUTEX));
	if (philos[i].id % 2 == 0)
	{
		philos[i].left_fork = &data->forks[(i + 1) % data->num_philos];
		philos[i].right_fork = &data->forks[i];
	}
	else
	{
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
	}
	return (TRUE);
}

static t_bool	init_philosophers(t_philo **philos, t_data *data)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!*philos)
		return (error_init_msg(ERR_MALLOC));
	i = 0;
	while (i < data->num_philos)
	{
		if (init_philo_entry(*philos, data, i) == FALSE)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

t_bool	init_data(t_data *data, t_philo **philos)
{
	data->dead = FALSE;
	data->start_time = 0;
	if (init_mutexes(data) == FALSE)
		return (FALSE);
	if (init_forks(data) == FALSE)
		return (FALSE);
	if (init_philosophers(philos, data) == FALSE)
		return (FALSE);
	return (TRUE);
}
