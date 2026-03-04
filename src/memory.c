/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:01:16 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:01:23 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	free_mutex(t_philo *start)
{
	if (!start->mutex)
		return ;
	if (start->mutex->fork)
		free(start->mutex->fork);
	free(start->mutex);
}

static void	free_nodes(t_philo *current, int total, int start_i)
{
	t_philo	*to_free;
	int		i;

	i = start_i;
	while (i < total && current)
	{
		to_free = current;
		current = current->next;
		free(to_free);
		i++;
	}
}

void	free_list(t_philo **begin_list)
{
	t_philo	*start;
	int		total;

	if (!begin_list || !*begin_list)
		return ;
	start = *begin_list;
	total = start->rules->ph_quantity;
	free_mutex(start);
	free_nodes(start->next, total, 1);
	free(start);
	*begin_list = NULL;
}
