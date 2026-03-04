/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wedos-sa <wedos-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:02:41 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/12/19 11:46:56 by wedos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
** Initializes the shared mutexes (meal, max_meal, time, write, dead).
** Called once before threads are launched.
*/
void	init_main(t_node *nodes)
{
	pthread_mutex_init(&nodes->mutex->meal_lock, NULL);
	pthread_mutex_init(&nodes->mutex->max_meal_lock, NULL);
	pthread_mutex_init(&nodes->mutex->time_lock, NULL);
	pthread_mutex_init(&nodes->mutex->write_lock, NULL);
	pthread_mutex_init(&nodes->mutex->dead, NULL);
}

/*
** Records the simulation start time and sets every philosopher's
** last_meal timestamp and meal counter to their initial values.
*/
void	init_timers(t_main *p)
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

/*
** Fills the rules struct with the simulation parameters from argv.
** flag == 1 → reset dead/start_time; flag == 2 → parse timing values.
*/
static void	init_rules(t_rules *rules, int flag, char **argv)
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

static void	error_free(t_mutex *mutex)
{
	free(mutex);
	exit(EXIT_FAILURE);
}

/*
** Allocates the mutex block and the fork array, initializes every fork,
** then builds the circular philosopher list.
*/
void	init_philo(t_rules *rules, char **argv, t_node **nodes)
{
	t_p	p;

	init_rules(rules, 1, argv);
	p.mutex = malloc(sizeof(t_mutex));
	if (!p.mutex)
		exit(EXIT_FAILURE);
	rules->ph_quantity = ft_atoi(argv[1]);
	p.mutex->hashi = malloc(sizeof(pthread_mutex_t) * rules->ph_quantity);
	if (!p.mutex->hashi)
		error_free(p.mutex);
	init_rules(rules, 2, argv);
	p.hashi_index = 0;
	while (p.hashi_index < rules->ph_quantity)
	{
		pthread_mutex_init(&p.mutex->hashi[p.hashi_index], NULL);
		p.hashi_index++;
	}
	p.philosopher_index = 1;
	(*nodes) = create_elem(p.philosopher_index, rules, p.mutex);
	p.philosopher_index++;
	while (p.philosopher_index <= rules->ph_quantity)
	{
		append_item(nodes, p.philosopher_index, rules, p.mutex);
		p.philosopher_index++;
	}
}

/*
** Assigns left/right fork pointers to each node and spawns its thread.
** The last philosopher's right fork wraps back to the first philosopher's
** left fork, closing the circle.
*/
static void	assign_forks_and_spawn(t_node *node, t_node *begin_list,
								pthread_mutex_t *hashi, int index_mutex)
{
	if (node->next == begin_list)
	{
		node->left = &hashi[index_mutex];
		node->right = begin_list->left;
	}
	else
	{
		node->left = &hashi[index_mutex];
		node->right = &hashi[index_mutex + 1];
	}
	pthread_create(&node->thread_id, NULL, routine, node);
}

void	threads_and_mutexes(t_node **nodes)
{
	t_node			*node;
	t_node			*begin_list;
	int				index_mutex;
	pthread_mutex_t	*hashi;

	begin_list = (*nodes);
	index_mutex = 0;
	node = *nodes;
	hashi = node->mutex->hashi;
	while (node)
	{
		assign_forks_and_spawn(node, begin_list, hashi, index_mutex);
		node = node->next;
		if (node == begin_list)
			break ;
		index_mutex++;
	}
}
