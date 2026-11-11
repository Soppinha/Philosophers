/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:38:57 by sofia             #+#    #+#             */
/*   Updated: 2026/03/16 18:09:39 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philosopher(t_philo *node)
{
	pthread_mutex_lock(node->left);
	pthread_mutex_lock(&node->mutex->write_lock);
	if (!node->rules->dead)
	{
		printf("%ld 1 has taken a fork\n",
			get_time() - node->rules->real_time);
	}
	pthread_mutex_unlock(&node->mutex->write_lock);
	usleep(node->rules->time_to_die * 1000);
	pthread_mutex_lock(&node->mutex->write_lock);
	if (!node->rules->dead)
	{
		printf("%ld 1 died\n",
			get_time() - node->rules->real_time);
	}
	pthread_mutex_unlock(&node->mutex->write_lock);
	pthread_mutex_unlock(node->left);
}
