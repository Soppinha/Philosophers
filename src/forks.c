/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:50:55 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:47:38 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_fork(t_philo *ptr)
{
	if (ptr->number % 2 == 0)
		return (take_left_fork(ptr));
	else
		return (take_right_fork(ptr));
}

void	put_fork(t_philo *ptr)
{
	pthread_mutex_unlock(ptr->left);
	pthread_mutex_unlock(ptr->right);
}

int	take_right_fork(t_philo *ptr)
{
	pthread_mutex_lock(ptr->right);
	if (is_dead(ptr))
	{
		pthread_mutex_unlock(ptr->right);
		return (FALSE);
	}
	pthread_mutex_lock(ptr->left);
	if (is_dead(ptr))
	{
		put_fork(ptr);
		return (FALSE);
	}
	return (TRUE);
}

int	take_left_fork(t_philo *ptr)
{
	pthread_mutex_lock(ptr->left);
	if (is_dead(ptr))
	{
		pthread_mutex_unlock(ptr->left);
		return (FALSE);
	}
	pthread_mutex_lock(ptr->right);
	if (is_dead(ptr))
	{
		put_fork(ptr);
		return (FALSE);
	}
	return (TRUE);
}
