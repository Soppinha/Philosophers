/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:00:00 by sopinha           #+#    #+#             */
/*   Updated: 2026/02/28 19:30:14 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_bool	is_simulation_over(t_philo *philo)
{
	t_bool	dead;

	pthread_mutex_lock(&philo->data->dead_lock);
	dead = philo->data->dead;
	pthread_mutex_unlock(&philo->data->dead_lock);
	return (dead);
}

void	ft_usleep(long milliseconds)
{
	long	start;
	long	remaining;

	start = get_time();
	while ((get_time() - start) < milliseconds * 1000)
	{
		remaining = milliseconds * 1000 - (get_time() - start);
		if (remaining > 1000)
			usleep(500);
		else
			usleep(100);
	}
}
