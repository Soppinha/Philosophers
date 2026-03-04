/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 16:51:14 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 17:08:31 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_main(t_node *nodes)
{
	pthread_mutex_init(&nodes->mutex->meal_lock, NULL);
	pthread_mutex_init(&nodes->mutex->max_meal_lock, NULL);
	pthread_mutex_init(&nodes->mutex->time_lock, NULL);
	pthread_mutex_init(&nodes->mutex->write_lock, NULL);
	pthread_mutex_init(&nodes->mutex->dead, NULL);
}

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

static void	free_mutex(t_mutex *mutex)
{
	free(mutex);
}

int	init_philo(t_rules *rules, char **argv, t_node **nodes)
{
	t_p	p;

	init_rules(rules, 1, argv);
	p.mutex = malloc(sizeof(t_mutex));
	if (!p.mutex)
		return (print_init_error(ERR_MALLOC));
	rules->ph_quantity = ft_atoi(argv[1]);
	p.mutex->hashi = malloc(sizeof(pthread_mutex_t) * rules->ph_quantity);
	if (!p.mutex->hashi)
	{
		free_mutex(p.mutex);
		return (print_init_error(ERR_MALLOC));
	}
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
	return (1);
}

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
