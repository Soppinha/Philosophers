/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wedos-sa <wedos-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:32 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/12/20 11:29:19 by wedos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* ─── Actions ─────────────────────────────────────────────────────────────── */

void	eat(t_node *ptr)
{
	pthread_mutex_lock(&ptr->mutex->meal_lock);
	ptr->last_meal = get_time();
	pthread_mutex_unlock(&ptr->mutex->meal_lock);
	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
	{
		printf(" %lu ", get_time() - ptr->rules->real_time);
		printf("|🥢🍜 philosopher %d is eating\n", ptr->number);
		printf("|====================================================|\n");
	}
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	p_sleep(ptr, 1);
	pthread_mutex_lock(&ptr->mutex->meal_lock);
	ptr->meals++;
	pthread_mutex_unlock(&ptr->mutex->meal_lock);
}

void	philosophers_sleep(t_node *ptr)
{
	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
	{
		printf(" %lu ", get_time() - ptr->rules->real_time);
		printf("|😴💤 philosopher %d is sleeping\n", ptr->number);
		printf("|====================================================|\n");
	}
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	p_sleep(ptr, 2);
}

void	think(t_node *ptr)
{
	if (is_dead(ptr))
		return ;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	if (!is_dead(ptr))
	{
		printf(" %lu ", get_time() - ptr->rules->real_time);
		printf("|🤯💬 philosopher %d is thinking\n", ptr->number);
		printf("|====================================================|\n");
	}
	pthread_mutex_unlock(&ptr->mutex->write_lock);
	if (ptr->number % 2 != 0 && ptr->rules->ph_quantity % 2 != 0)
		usleep(1000);
}

/* ─── Main loop ───────────────────────────────────────────────────────────── */

void	routine_while(t_node *node)
{
	int	has_forks;

	while (!is_dead(node))
	{
		has_forks = 0;
		if (take_hashis(node))
			has_forks = 1;
		if (is_dead(node))
		{
			if (has_forks)
				put_hashis(node);
			break ;
		}
		eat(node);
		put_hashis(node);
		if (is_dead(node))
			break ;
		philosophers_sleep(node);
		if (is_dead(node))
			break ;
		think(node);
	}
}

/* ─── Edge case: single philosopher ──────────────────────────────────────── */

void	one_philosopher(t_node *node)
{
	pthread_mutex_lock(node->left);
	pthread_mutex_lock(&node->mutex->write_lock);
	if (!node->rules->dead)
	{
		printf(" %lu |🥢 philosopher 1 has taken a fork\n",
			get_time() - node->rules->real_time);
	}
	pthread_mutex_unlock(&node->mutex->write_lock);
	usleep(node->rules->time_to_die * 1000);
	pthread_mutex_lock(&node->mutex->write_lock);
	if (!node->rules->dead)
	{
		printf(" %lu | philosopher 1 died ☠️\n",
			get_time() - node->rules->real_time);
	}
	pthread_mutex_unlock(&node->mutex->write_lock);
	pthread_mutex_unlock(node->left);
}

/* ─── Thread entry point ─────────────────────────────────────────────────── */

static void	wait_start(t_node *node)
{
	while (1)
	{
		if (node->rules->start_time != 0)
			break ;
		usleep(50);
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
