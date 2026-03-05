/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:09:59 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:47:30 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	do_eat_cycle(t_philo *node)
{
	if (!take_fork(node))
		return (FALSE);
	if (is_dead(node))
	{
		put_fork(node);
		return (FALSE);
	}
	pthread_mutex_lock(&node->mutex->write_lock);
	if (is_dead(node))
	{
		pthread_mutex_unlock(&node->mutex->write_lock);
		put_fork(node);
		return (FALSE);
	}
	print_take_fork(node);
	print_take_fork(node);
	pthread_mutex_unlock(&node->mutex->write_lock);
	eating(node);
	put_fork(node);
	return (TRUE);
}

void	routine_while(t_philo *node)
{
	while (!is_dead(node))
	{
		if (!do_eat_cycle(node))
			break ;
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
	t_philo	*node;

	node = (t_philo *)ptr;
	if (node->rules->ph_quantity == 1)
	{
		one_philosopher(node);
		return (NULL);
	}
	wait_start(node);
	routine_while(ptr);
	return (NULL);
}
