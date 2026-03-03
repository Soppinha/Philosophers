/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:58:53 by sopinha           #+#    #+#             */
/*   Updated: 2026/03/03 19:57:33 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks(t_philo *philo, t_fork **first, t_fork **second)
{
	if (philo->left_fork->id < philo->right_fork->id)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	pthread_mutex_lock(&(*first)->mutex);
	pthread_mutex_lock(&(*second)->mutex);
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(&(*second)->mutex);
		pthread_mutex_unlock(&(*first)->mutex);
		return (0);
	}
	return (1);
}

void	philo_eat(t_philo *philo)
{
	t_fork	*first;
	t_fork	*second;

	if (is_simulation_over(philo))
		return ;
	if (!take_forks(philo, &first, &second))
		return ;
	print_status(philo, FORK_TAKEN);
	print_status(philo, FORK_TAKEN);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status(philo, EATING);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}

void	philo_sleep(t_philo *philo)
{
	if (is_simulation_over(philo))
		return ;
	print_status(philo, SLEEPING);
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	if (is_simulation_over(philo))
		return ;
	print_status(philo, THINKING);
}
