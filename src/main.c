/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:09:31 by sofia             #+#    #+#             */
/*   Updated: 2026/03/16 18:09:28 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_all_threads(t_philo **nodes)
{
	t_philo	*start;
	t_philo	*curr;

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

void	destroy_mutexes(t_philo *nodes)
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
		pthread_mutex_destroy(&nodes->mutex->fork[i]);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_simu	p;

	if (!init_check(argc, argv))
		return (1);
	if (!init_philo(&p.rules, argv, &p.nodes))
		return (1);
	init_simu(p.nodes);
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
