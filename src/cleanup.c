/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:33:49 by sopinha           #+#    #+#             */
/*   Updated: 2026/03/03 19:13:17 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i].mutex);
		i++;
	}
}

static void	destroy_philos_mutexes(t_philo *philos, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&philos[i].meal_mutex);
		i++;
	}
}

void	cleanup(t_data *data, t_philo *philos)
{
	if (data->forks)
	{
		destroy_forks(data);
		free(data->forks);
		data->forks = NULL;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->dead_lock);
	pthread_mutex_destroy(&data->meal_lock);
	if (philos)
	{
		destroy_philos_mutexes(philos, data->num_philos);
		free(philos);
		philos = NULL;
	}
}
