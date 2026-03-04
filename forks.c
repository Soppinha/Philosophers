/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wedos-sa <wedos-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:43 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/12/20 11:27:34 by wedos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* ─── Print helpers ───────────────────────────────────────────────────────── */

void	print_fork_right(t_node *ptr)
{
	pthread_mutex_lock(&ptr->mutex->write_lock);
	printf(" %lu ", get_time() - ptr->rules->real_time);
	printf("|🥢👈 philosopher %d has taken a fork\n", ptr->number);
	printf("|====================================================|\n");
	pthread_mutex_unlock(&ptr->mutex->write_lock);
}

void	print_fork_left(t_node *ptr)
{
	pthread_mutex_lock(&ptr->mutex->write_lock);
	printf(" %lu ", get_time() - ptr->rules->real_time);
	printf("|🥢👈 philosopher %d has taken a fork\n", ptr->number);
	printf("|====================================================|\n");
	pthread_mutex_unlock(&ptr->mutex->write_lock);
}

/* ─── Fork acquisition ────────────────────────────────────────────────────── */

/*
** Even-numbered philosophers grab their left fork first to avoid deadlock.
** Odd-numbered philosophers grab their right fork first.
*/
int	take_hashis(t_node *ptr)
{
	if (ptr->number % 2 == 0)
		return (take_left_hashi(ptr));
	else
		return (take_right_hashi(ptr));
}

void	put_hashis(t_node *ptr)
{
	pthread_mutex_unlock(ptr->left);
	pthread_mutex_unlock(ptr->right);
}

int	take_right_hashi(t_node *ptr)
{
	pthread_mutex_lock(ptr->right);
	if (is_dead(ptr))
	{
		pthread_mutex_unlock(ptr->right);
		return (0);
	}
	print_fork_right(ptr);
	pthread_mutex_lock(ptr->left);
	if (is_dead(ptr))
	{
		pthread_mutex_unlock(ptr->left);
		pthread_mutex_unlock(ptr->right);
		return (0);
	}
	print_fork_left(ptr);
	return (1);
}

int	take_left_hashi(t_node *ptr)
{
	pthread_mutex_lock(ptr->left);
	if (is_dead(ptr))
	{
		pthread_mutex_unlock(ptr->left);
		return (0);
	}
	print_fork_left(ptr);
	pthread_mutex_lock(ptr->right);
	if (is_dead(ptr))
	{
		pthread_mutex_unlock(ptr->right);
		pthread_mutex_unlock(ptr->left);
		return (0);
	}
	print_fork_right(ptr);
	return (1);
}
