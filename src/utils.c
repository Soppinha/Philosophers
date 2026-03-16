/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:10:12 by sofia             #+#    #+#             */
/*   Updated: 2026/03/16 18:09:53 by svaladar         ###   ########.fr       */
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

int	is_dead(t_philo *ptr)
{
	int	dead;

	pthread_mutex_lock(&ptr->mutex->dead);
	dead = ptr->rules->dead;
	pthread_mutex_unlock(&ptr->mutex->dead);
	return (dead);
}

void	wait_start(t_philo *node)
{
	while (1)
	{
		if (node->rules->start_time != 0)
			break ;
		usleep(50);
	}
}
