/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:38:57 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 17:00:48 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philosopher(t_node *node)
{
	pthread_mutex_lock(node->left);
	pthread_mutex_lock(&node->mutex->write_lock);
	if (!node->rules->dead)
	{
		printf(" %lu |🍴 philosopher 1 has taken a fork\n",
			get_time() - node->rules->real_time);
	}
	pthread_mutex_unlock(&node->mutex->write_lock);
	usleep(node->rules->time_to_die * 1000);
	pthread_mutex_lock(&node->mutex->write_lock);
	if (!node->rules->dead)
	{
		printf(" %lu | philosopher 1 died 👻\n",
			get_time() - node->rules->real_time);
	}
	pthread_mutex_unlock(&node->mutex->write_lock);
	pthread_mutex_unlock(node->left);
}
