/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wedos-sa <wedos-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:04 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/12/20 11:28:18 by wedos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* ─── Reporting ───────────────────────────────────────────────────────────── */

void	philo_eat_print(t_node *ptr, t_node *temp)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	printf(" %lu | ✅ all philosophers have eaten\n",
		get_time() - ptr->rules->real_time);
	printf("|==================================================|\n");
	temp = ptr;
	while (i < ptr->rules->ph_quantity)
	{
		printf(" ✅ philosopher %d eat [%d]x\n", temp->number, temp->meals);
		temp = temp->next;
		i++;
	}
	printf("|==================================================|\n");
	pthread_mutex_unlock(&ptr->mutex->write_lock);
}

static void	print_dead(t_node *ptr)
{
	pthread_mutex_lock(&ptr->mutex->write_lock);
	printf(" %ld | philosopher %d dead ☠️\n",
		get_time() - ptr->rules->real_time, ptr->number);
	pthread_mutex_unlock(&ptr->mutex->write_lock);
}

/* ─── Meal saturation check ───────────────────────────────────────────────── */

/*
** Walks the full circle once. If every philosopher has reached max_meals,
** prints the summary and sets the dead flag to end the simulation.
*/
void	eat_monitor(t_node *ptr)
{
	t_node	*begin_list;
	t_node	*temp;

	begin_list = ptr;
	temp = ptr;
	while (temp)
	{
		pthread_mutex_lock(&temp->mutex->meal_lock);
		if (temp->meals < temp->rules->max_meals)
		{
			pthread_mutex_unlock(&temp->mutex->meal_lock);
			return ;
		}
		pthread_mutex_unlock(&temp->mutex->meal_lock);
		temp = temp->next;
		if (temp == begin_list)
			break ;
	}
	philo_eat_print(ptr, temp);
	pthread_mutex_lock(&ptr->mutex->dead);
	ptr->rules->dead = 1;
	pthread_mutex_unlock(&ptr->mutex->dead);
}

/* ─── Per-iteration monitor pass ──────────────────────────────────────────── */

/*
** Checks each philosopher in one pass:
** - If max_meals is set, verifies the saturation condition first.
** - Then checks whether time_to_die has elapsed since the last meal.
** Returns NULL if the simulation must stop, (void *)1 otherwise.
*/
void	*monitor_looping(t_node *ptr, t_node *begin_list)
{
	while (ptr && ptr->next != begin_list)
	{
		if (ptr->rules->max_meals > 0)
		{
			pthread_mutex_lock(&ptr->mutex->max_meal_lock);
			eat_monitor(ptr);
			pthread_mutex_unlock(&ptr->mutex->max_meal_lock);
			if (is_dead(ptr))
				return (NULL);
		}
		pthread_mutex_lock(&ptr->mutex->meal_lock);
		if (get_time() - ptr->last_meal > ptr->rules->time_to_die)
		{
			pthread_mutex_unlock(&ptr->mutex->meal_lock);
			print_dead(ptr);
			pthread_mutex_lock(&ptr->mutex->dead);
			ptr->rules->dead = 1;
			pthread_mutex_unlock(&ptr->mutex->dead);
			return (NULL);
		}
		pthread_mutex_unlock(&ptr->mutex->meal_lock);
		ptr = ptr->next;
	}
	return ((void *)1);
}

/* ─── Monitor thread ──────────────────────────────────────────────────────── */

void	*monitor(void *head)
{
	t_node	*ptr;
	t_node	*begin_list;

	ptr = (t_node *)head;
	begin_list = ptr;
	pthread_mutex_lock(&ptr->mutex->dead);
	ptr->rules->dead = 0;
	pthread_mutex_unlock(&ptr->mutex->dead);
	while (!is_dead(ptr))
	{
		ptr = begin_list;
		if (monitor_looping(ptr, begin_list) == NULL)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
