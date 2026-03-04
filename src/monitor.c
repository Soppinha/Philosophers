/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:09:42 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:29:44 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat_print(t_philo *ptr, t_philo *temp)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&ptr->mutex->write_lock);
	printf("\n");
	printf("⭐⭐⭐	all philosophers have eaten ⭐⭐⭐\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	temp = ptr;
	while (i < ptr->rules->ph_quantity)
	{
		printf(" 🧓 philosopher %d 🍔 %dx\n", temp->number, temp->meals);
		temp = temp->next;
		i++;
	}
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	pthread_mutex_unlock(&ptr->mutex->write_lock);
}

void	eat_monitor(t_philo *begin_list)
{
	t_philo	*temp;

	temp = begin_list;
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
	philo_eat_print(begin_list, temp);
	pthread_mutex_lock(&begin_list->mutex->dead);
	begin_list->rules->dead = 1;
	pthread_mutex_unlock(&begin_list->mutex->dead);
}

static int	check_death(t_philo *ptr)
{
	pthread_mutex_lock(&ptr->mutex->meal_lock);
	if (get_time() - ptr->last_meal > ptr->rules->time_to_die)
	{
		pthread_mutex_lock(&ptr->mutex->dead);
		ptr->rules->dead = 1;
		pthread_mutex_unlock(&ptr->mutex->dead);
		pthread_mutex_unlock(&ptr->mutex->meal_lock);
		pthread_mutex_lock(&ptr->mutex->write_lock);
		print_dead(ptr);
		pthread_mutex_unlock(&ptr->mutex->write_lock);
		return (TRUE);
	}
	pthread_mutex_unlock(&ptr->mutex->meal_lock);
	return (FALSE);
}

void	*monitor_looping(t_philo *ptr, t_philo *begin_list)
{
	int	first;

	if (ptr->rules->max_meals > 0)
	{
		pthread_mutex_lock(&ptr->mutex->max_meal_lock);
		eat_monitor(begin_list);
		pthread_mutex_unlock(&ptr->mutex->max_meal_lock);
		if (is_dead(ptr))
			return (NULL);
	}
	first = 1;
	while (ptr && (first || ptr != begin_list))
	{
		first = 0;
		if (check_death(ptr))
			return (NULL);
		ptr = ptr->next;
	}
	return ((void *)1);
}

void	*monitor(void *head)
{
	t_philo	*ptr;
	t_philo	*begin_list;

	ptr = (t_philo *)head;
	begin_list = ptr;
	pthread_mutex_lock(&ptr->mutex->dead);
	ptr->rules->dead = 0;
	pthread_mutex_unlock(&ptr->mutex->dead);
	while (!is_dead(ptr))
	{
		ptr = begin_list;
		if (monitor_looping(ptr, begin_list) == NULL)
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
