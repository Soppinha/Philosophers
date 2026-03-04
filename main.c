/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wedos-sa <wedos-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:02:57 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/12/19 11:46:56 by wedos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	p_sleep(t_node *ptr, int flag)
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

void	join_all_threads(t_node **nodes)
{
	t_node	*start;
	t_node	*curr;

	start = (*nodes);
	curr = (*nodes);
	if (!curr)
		return ;
	while (1)
	{
		pthread_join(curr->thread_id, NULL);
		curr = curr->next;
		if (curr == start)
			break ;
	}
}

void	destroy_mutexes(t_node *nodes)
{
	int	i;

	if (!nodes)
		return ;
	pthread_mutex_destroy(&nodes->mutex->meal_lock);
	pthread_mutex_destroy(&nodes->mutex->max_meal_lock);
	pthread_mutex_destroy(&nodes->mutex->time_lock);
	pthread_mutex_destroy(&nodes->mutex->write_lock);
	pthread_mutex_destroy(&nodes->mutex->dead);
	i = 0;
	while (i < nodes->rules->ph_quantity)
	{
		pthread_mutex_destroy(&nodes->mutex->hashi[i]);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_main	p;

	init_check(argc, argv);
	init_philo(&p.rules, argv, &p.nodes);
	init_main(p.nodes);
	init_timers(&p);
	threads_and_mutexes(&p.nodes);
	if (p.nodes->rules->ph_quantity == 1)
		pthread_join(p.nodes->thread_id, NULL);
	else
	{
		pthread_create(&p.nodes->rules->monitor, NULL, monitor, p.nodes);
		pthread_join(p.nodes->rules->monitor, NULL);
		join_all_threads(&p.nodes);
	}
	usleep(1000);
	destroy_mutexes(p.nodes);
	free_list(&p.nodes);
	return (0);
}
