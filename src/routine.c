/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:09:59 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:01:23 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	do_eat_cycle(t_philo *node)
{
	int	has_forks;

	has_forks = 0;
	if (take_fork(node))
		has_forks = 1;
	if (is_dead(node))
	{
		if (has_forks)
			put_fork(node);
		return (FALSE);
	}
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
