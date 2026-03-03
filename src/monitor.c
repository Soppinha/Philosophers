/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 14:15:02 by sopinha           #+#    #+#             */
/*   Updated: 2026/03/03 19:14:44 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->dead_lock);
	data->dead = TRUE;
	pthread_mutex_unlock(&data->dead_lock);
}

static t_bool	philo_has_died(t_philo *philo)
{
	long	time_since_meal;
	int		must_eat;

	pthread_mutex_lock(&philo->data->meal_lock);
	must_eat = philo->data->num_must_eat;
	pthread_mutex_unlock(&philo->data->meal_lock);
	pthread_mutex_lock(&philo->meal_mutex);
	if (must_eat != -1 && philo->meals_eaten >= must_eat)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return (FALSE);
	}
	time_since_meal = get_time() - philo->last_meal_time;
	if (time_since_meal > philo->data->time_to_die * 1000)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		stop_simulation(philo->data);
		print_status(philo, DEAD);
		return (TRUE);
	}
	pthread_mutex_unlock(&philo->meal_mutex);
	return (FALSE);
}

static t_bool	all_philos_satisfied(t_philo *philos, t_data *data)
{
	int	i;
	int	must_eat;

	pthread_mutex_lock(&data->meal_lock);
	must_eat = data->num_must_eat;
	pthread_mutex_unlock(&data->meal_lock);
	if (must_eat == -1)
		return (FALSE);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&philos[i].meal_mutex);
		if (philos[i].meals_eaten < must_eat)
		{
			pthread_mutex_unlock(&philos[i].meal_mutex);
			return (FALSE);
		}
		pthread_mutex_unlock(&philos[i].meal_mutex);
		i++;
	}
	stop_simulation(data);
	return (TRUE);
}

t_bool	is_simulation_over(t_philo *philo)
{
	t_bool	dead;

	pthread_mutex_lock(&philo->data->dead_lock);
	dead = philo->data->dead;
	pthread_mutex_unlock(&philo->data->dead_lock);
	return (dead);
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
			if (philo_has_died(&philos[i]))
				return (NULL);
			i++;
		}
		if (all_philos_satisfied(philos, philos[0].data))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
