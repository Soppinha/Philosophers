/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:01:16 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 21:05:17 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_philo *ptr)
{
	pthread_mutex_lock(&ptr->mutex->meal_lock);
	ptr->last_meal = get_time();
	ptr->meals++;
	pthread_mutex_unlock(&ptr->mutex->meal_lock);
	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
		print_eating(ptr);
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	p_sleep(ptr, 1);
}

void	sleeping(t_philo *ptr)
{
	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
		print_sleeping(ptr);
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	p_sleep(ptr, 2);
}

void	thinking(t_philo *ptr)
{
	long	delay;

	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
		print_thinking(ptr);
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	if (ptr->rules->ph_quantity % 2 == 0)
		return ;
	delay = ptr->rules->time_to_die
		- ptr->rules->time_to_eat
		- ptr->rules->time_to_sleep;
	if (delay > ptr->rules->time_to_eat)
		usleep(delay * 500);
}
