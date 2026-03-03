/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:17:41 by sopinha           #+#    #+#             */
/*   Updated: 2026/03/03 15:57:17 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	create_threads(t_philo *philos, pthread_t *monitor)
{
	int	i;

	i = 0;
	while (i < philos[0].data->num_philos)
	{
		if (pthread_create(&philos[i].thread, NULL,
				philosopher_routine, &philos[i]) != 0)
			return (error_init_msg(ERR_THREAD));
		i++;
	}
	if (pthread_create(monitor, NULL, monitor_routine, philos) != 0)
		return (error_init_msg(ERR_THREAD));
	return (TRUE);
}

static void	join_threads(t_philo *philos, pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < philos[0].data->num_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

t_bool	start_simulation(t_data *data, t_philo *philos)
{
	pthread_t	monitor;
	int			i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		philos[i].last_meal_time = data->start_time;
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	if (create_threads(philos, &monitor) == FALSE)
		return (FALSE);
	join_threads(philos, monitor);
	return (TRUE);
}
