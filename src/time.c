/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 17:01:16 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:01:23 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	p_sleep(t_philo *ptr, int flag)
{
	long	start;

	start = get_time();
	if (flag == 1)
	{
		while (get_time() - start < ptr->rules->time_to_eat)
		{
			if (is_dead(ptr))
				break ;
			usleep(500);
		}
	}
	else if (flag == 2)
	{
		while (get_time() - start < ptr->rules->time_to_sleep)
		{
			if (is_dead(ptr))
				break ;
			usleep(500);
		}
	}
}
