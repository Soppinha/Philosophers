/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:15:02 by sopinha           #+#    #+#             */
/*   Updated: 2026/03/03 16:05:10 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_dead_flag(t_data *data)
{
	pthread_mutex_lock(&data->dead_lock);
	data->dead = TRUE;
	pthread_mutex_unlock(&data->dead_lock);
}

static t_bool	check_philosopher_death(t_philo *philo)
{
	long	time_since_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	if (philo->data->num_must_eat != -1
		&& philo->meals_eaten >= philo->data->num_must_eat)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return (FALSE);
	}
	time_since_meal = get_time() - philo->last_meal_time;
	if (time_since_meal > philo->data->time_to_die * 1000)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		set_dead_flag(philo->data);
		print_status(philo, DEAD);
		return (TRUE);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (FALSE);
}

static t_bool	check_all_ate_enough(t_philo *philos, t_data *data)
{
	int	i;

	if (data->num_must_eat == -1)
		return (FALSE);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].meals_eaten < data->num_must_eat)
		{
			pthread_mutex_unlock(&philos[i].meal_mutex);
			return (FALSE);
		}
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	set_dead_flag(data);
	return (TRUE);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)arg;
	while (TRUE)
	{
		i = 0;
		while (i < philos[0].data->num_philos)
		{
			if (check_philosopher_death(&philos[i]))
				return (NULL);
			i++;
		}
		if (check_all_ate_enough(philos, philos[0].data))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
