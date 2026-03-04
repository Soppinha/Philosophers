/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:43 by wedos-sa          #+#    #+#             */
/*   Updated: 2026/03/04 17:00:54 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *string)
{
	int					i;
	int					sign;
	unsigned long int	nb;

	nb = 0;
	i = 0;
	sign = 1;
	while ((string[i] >= 9 && string[i] <= 13) || string[i] == 32)
		i++;
	if (string[i] == '-' || string[i] == '+')
	{
		if (string[i] == '-')
			sign = -1;
		if (string[i++] < '0' && string[i] > '9')
			return (0);
	}
	while (string[i] >= '0' && string[i] <= '9')
	{
		nb = nb * 10 + (string[i] - 48);
		i++;
	}
	return (nb * sign);
}

int	is_dead(t_node *ptr)
{
	int	dead;

	pthread_mutex_lock(&ptr->mutex->dead);
	dead = ptr->rules->dead;
	pthread_mutex_unlock(&ptr->mutex->dead);
	return (dead);
}

void	wait_start(t_node *node)
{
	while (1)
	{
		if (node->rules->start_time != 0)
			break ;
		usleep(50);
	}
}

void	free_list(t_node **begin_list)
{
	t_free	p;

	if (!begin_list || !*begin_list)
		return ;
	p.start = *begin_list;
	p.total_nodes = p.start->rules->ph_quantity;
	p.current = p.start->next;
	if (p.start->mutex)
	{
		if (p.start->mutex->hashi)
			free(p.start->mutex->hashi);
		free(p.start->mutex);
	}
	free(p.start);
	p.i = 1;
	while (p.i < p.total_nodes && p.current)
	{
		p.to_free = p.current;
		p.current = p.current->next;
		free(p.to_free);
		p.i++;
	}
	*begin_list = NULL;
}
