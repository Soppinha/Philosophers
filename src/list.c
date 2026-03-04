/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:09:21 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:01:23 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*create_elem(int philosopher_number, t_config *rules, t_locks *mutex)
{
	t_philo	*ptr;

	ptr = malloc(sizeof(t_philo));
	if (!ptr)
		return (NULL);
	ptr->number = philosopher_number;
	ptr->next = NULL;
	ptr->prev = NULL;
	ptr->rules = rules;
	ptr->mutex = mutex;
	return (ptr);
}

int	list_size(t_philo *begin_list)
{
	t_philo	*index;
	int		i;

	index = begin_list;
	i = 0;
	while (index)
	{
		i++;
		index = index->next;
	}
	return (i);
}

t_philo	*list_last(t_philo *head)
{
	t_philo	*node;

	if (!head)
		return (NULL);
	node = head;
	while (node->next != head && node->next != NULL)
		node = node->next;
	return (node);
}

void	append_item(t_philo **ptr, int ph_nb, t_config *rules, t_locks *mutex)
{
	t_philo	*last;
	t_philo	*item;

	if (!ptr)
		return ;
	item = create_elem(ph_nb, rules, mutex);
	if (!item)
		return ;
	if (!*ptr)
	{
		*ptr = item;
		return ;
	}
	last = list_last(*ptr);
	last->next = item;
	last->next->prev = last;
	(*ptr)->prev = last->next;
	last->next->next = *ptr;
}
