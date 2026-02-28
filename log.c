/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:49:20 by sopinha           #+#    #+#             */
/*   Updated: 2026/02/28 18:15:01 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_status_msg(t_philo *philo, t_status status)
{
	long	ms;

	ms = (get_time() - philo->data->start_time) / 1000;
	if (status == FORK_TAKEN)
		printf("%ld %d has taken a fork\n", ms, philo->id);
	else if (status == EATING)
		printf("%ld %d is eating\n", ms, philo->id);
	else if (status == SLEEPING)
		printf("%ld %d is sleeping\n", ms, philo->id);
	else if (status == THINKING)
		printf("%ld %d is thinking\n", ms, philo->id);
	else if (status == DEAD)
		printf("%ld %d died\n", ms, philo->id);
}

void	print_status(t_philo *philo, t_status status)
{
	pthread_mutex_lock(&philo->data->write_lock);
	if (!is_simulation_over(philo) || status == DEAD)
		print_status_msg(philo, status);
	pthread_mutex_unlock(&philo->data->write_lock);
}
