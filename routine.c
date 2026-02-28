/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 02:58:30 by sopinha           #+#    #+#             */
/*   Updated: 2026/02/27 05:00:00 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
		return (one_philo_routine(arg));
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 2);
	while (!is_simulation_over(philo))
	{
		philo_eat(philo);
		if (is_simulation_over(philo))
			break ;
		philo_sleep(philo);
		if (is_simulation_over(philo))
			break ;
		philo_think(philo);
	}
	return (NULL);
}
