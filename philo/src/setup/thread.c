/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:51:14 by sofia             #+#    #+#             */
/*   Updated: 2026/03/16 18:09:47 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks_and_spawn(t_philo *node, t_philo *begin_list,
								pthread_mutex_t *fork, int index_mutex)
{
	if (node->next == begin_list)
	{
		node->left = &fork[index_mutex];
		node->right = begin_list->left;
	}
	else
	{
		node->left = &fork[index_mutex];
		node->right = &fork[index_mutex + 1];
	}
	pthread_create(&node->thread_id, NULL, routine, node);
}

void	threads_and_mutexes(t_philo **nodes)
{
	t_philo			*node;
	t_philo			*begin_list;
	int				index_mutex;
	pthread_mutex_t	*fork;

	begin_list = (*nodes);
	index_mutex = 0;
	node = *nodes;
	fork = node->mutex->fork;
	while (node)
	{
		assign_forks_and_spawn(node, begin_list, fork, index_mutex);
		node = node->next;
		if (node == begin_list)
			break ;
		index_mutex++;
	}
}
