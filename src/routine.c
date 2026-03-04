/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:32 by wedos-sa          #+#    #+#             */
/*   Updated: 2026/03/04 17:01:16 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eating(t_node *ptr)
{
	pthread_mutex_lock(&ptr->mutex->meal_lock);
	ptr->last_meal = get_time();
	pthread_mutex_unlock(&ptr->mutex->meal_lock);
	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
		print_eating(ptr);
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	p_sleep(ptr, 1);
	pthread_mutex_lock(&ptr->mutex->meal_lock);
	ptr->meals++;
	pthread_mutex_unlock(&ptr->mutex->meal_lock);
}

void	sleeping(t_node *ptr)
{
	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
		print_sleeping(ptr);
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	p_sleep(ptr, 2);
}

void	thinking(t_node *ptr)
{
	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
		print_thinking(ptr);
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	if (ptr->number % 2 != 0 && ptr->rules->ph_quantity % 2 != 0)
		usleep(1000);
}

void	routine_while(t_node *node)
{
	int	has_forks;

	while (!is_dead(node))
	{
		has_forks = 0;
		if (take_fork(node))
			has_forks = 1;
		if (is_dead(node))
		{
			if (has_forks)
				put_fork(node);
			break ;
		}
		eating(node);
		put_fork(node);
		if (is_dead(node))
			break ;
		sleeping(node);
		if (is_dead(node))
			break ;
		thinking(node);
	}
}

void	*routine(void *ptr)
{
	t_node	*node;

	node = (t_node *)ptr;
	if (node->rules->ph_quantity == 1)
	{
		one_philosopher(node);
		return (NULL);
	}
	wait_start(node);
	routine_while(ptr);
	return (NULL);
}
