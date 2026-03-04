/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:43:20 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 16:44:31 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_take_fork(t_node *ptr)
{
	pthread_mutex_lock(&ptr->mutex->write_lock);
	printf(" %lu ", get_time() - ptr->rules->real_time);
	printf("|🍴 philosopher %d has taken a fork\n", ptr->number);
	printf("\n");
	pthread_mutex_unlock(&ptr->mutex->write_lock);
}

void	print_eating(t_node *ptr)
{
	printf(" %lu ", get_time() - ptr->rules->real_time);
	printf("|🍔 philosopher %d is eating\n", ptr->number);
	printf("\n");
}

void	print_sleeping(t_node *ptr)
{
	printf(" %lu ", get_time() - ptr->rules->real_time);
	printf("|😴 philosopher %d is sleeping\n", ptr->number);
	printf("\n");
}

void	print_thinking(t_node *ptr)
{
	printf(" %lu ", get_time() - ptr->rules->real_time);
	printf("|🤔 philosopher %d is thinking\n", ptr->number);
	printf("\n");
}

void	print_dead(t_node *ptr)
{
	pthread_mutex_lock(&ptr->mutex->write_lock);
	printf(" %ld | philosopher %d dead 👻\n",
		get_time() - ptr->rules->real_time, ptr->number);
	pthread_mutex_unlock(&ptr->mutex->write_lock);
}
