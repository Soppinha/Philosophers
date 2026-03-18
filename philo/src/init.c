/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:51:14 by sofia             #+#    #+#             */
/*   Updated: 2026/03/16 18:09:07 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_simu(t_philo *nodes)
{
	pthread_mutex_init(&nodes->mutex->meal_lock, NULL);
	pthread_mutex_init(&nodes->mutex->max_meal_lock, NULL);
	pthread_mutex_init(&nodes->mutex->time_lock, NULL);
	pthread_mutex_init(&nodes->mutex->write_lock, NULL);
	pthread_mutex_init(&nodes->mutex->dead, NULL);
}

void	init_timers(t_simu *p)
{
	p->start = get_time();
	p->nodes->rules->start_time = p->start;
	p->nodes->rules->real_time = p->start;
	p->temp = p->nodes;
	p->first = 1;
	while (p->temp && (p->temp != p->nodes || p->first))
	{
		p->first = 0;
		pthread_mutex_lock(&p->temp->mutex->meal_lock);
		p->temp->last_meal = p->start;
		p->temp->meals = 0;
		pthread_mutex_unlock(&p->temp->mutex->meal_lock);
		p->temp = p->temp->next;
	}
}

static void	init_config(t_config *rules, int flag, char **argv)
{
	if (flag == 1)
	{
		rules->start_time = 0;
		rules->dead = 0;
		return ;
	}
	if (flag == 2)
	{
		rules->time_to_die = ft_atoi(argv[2]);
		rules->time_to_eat = ft_atoi(argv[3]);
		rules->time_to_sleep = ft_atoi(argv[4]);
		if (argv[5])
			rules->max_meals = ft_atoi(argv[5]);
		else
			rules->max_meals = -1;
	}
}

static int	init_forks(t_config *rules, t_init *p)
{
	p->mutex->fork = malloc(sizeof(pthread_mutex_t) * rules->ph_quantity);
	if (!p->mutex->fork)
	{
		free(p->mutex);
		return (print_init_error(ERR_MALLOC));
	}
	p->fork_index = 0;
	while (p->fork_index < rules->ph_quantity)
	{
		pthread_mutex_init(&p->mutex->fork[p->fork_index], NULL);
		p->fork_index++;
	}
	return (TRUE);
}

int	init_philo(t_config *rules, char **argv, t_philo **nodes)
{
	t_init	p;

	init_config(rules, 1, argv);
	p.mutex = malloc(sizeof(t_locks));
	if (!p.mutex)
		return (print_init_error(ERR_MALLOC));
	rules->ph_quantity = ft_atoi(argv[1]);
	if (!init_forks(rules, &p))
		return (FALSE);
	init_config(rules, 2, argv);
	p.philo_index = 1;
	(*nodes) = create_elem(p.philo_index, rules, p.mutex);
	p.philo_index++;
	while (p.philo_index <= rules->ph_quantity)
	{
		append_item(nodes, p.philo_index, rules, p.mutex);
		p.philo_index++;
	}
	return (TRUE);
}
