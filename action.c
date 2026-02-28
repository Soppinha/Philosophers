/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 13:58:53 by sopinha           #+#    #+#             */
/*   Updated: 2026/02/28 18:50:57 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex);
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (FALSE);
	}
	print_status(philo, FORK_TAKEN);
	pthread_mutex_lock(&philo->right_fork->mutex);
	if (is_simulation_over(philo))
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (FALSE);
	}
	print_status(philo, FORK_TAKEN);
	return (TRUE);
}

static t_bool	check_and_eat(t_philo *philo)
{
	long	elapsed;

	pthread_mutex_lock(&philo->meal_mutex);
	elapsed = get_time() - philo->last_meal_time;
	if (elapsed >= philo->data->time_to_die * 1000)
	{
		pthread_mutex_unlock(&philo->meal_mutex);
		return (FALSE);
	}
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (TRUE);
}

void	philo_eat(t_philo *philo)
{
	if (!take_forks(philo))
		return ;
	if (!check_and_eat(philo))
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		while (!is_simulation_over(philo))
			usleep(100);
		return ;
	}
	print_status(philo, EATING);
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, SLEEPING);
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, THINKING);
	if (philo->data->num_philos % 2 != 0)
	{
		think_time = (philo->data->time_to_die
				- (philo->data->time_to_eat
					+ philo->data->time_to_sleep)) / 2;
		if (think_time > 0 && think_time < 600)
			ft_usleep(think_time);
		else if (think_time >= 600)
			ft_usleep(200);
	}
}
