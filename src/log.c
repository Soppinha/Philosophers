/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:43:20 by sofia             #+#    #+#             */
/*   Updated: 2026/03/04 20:19:29 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_take_fork(t_philo *ptr)
{
	printf(" %lu 🔹 🍴 philosopher %d has taken a fork\n",
		get_time() - ptr->rules->real_time, ptr->number);
}

void	print_eating(t_philo *ptr)
{
	printf(" %lu 🔹 🍔 philosopher %d is eating\n",
		get_time() - ptr->rules->real_time, ptr->number);
}

void	print_sleeping(t_philo *ptr)
{
	printf(" %lu 🔹 😴 philosopher %d is sleeping\n",
		get_time() - ptr->rules->real_time, ptr->number);
}

void	print_thinking(t_philo *ptr)
{
	printf(" %lu 🔹 🤔 philosopher %d is thinking\n",
		get_time() - ptr->rules->real_time, ptr->number);
}

void	print_dead(t_philo *ptr)
{
	printf(" %ld 🔹 philosopher %d dead 👻\n",
		get_time() - ptr->rules->real_time, ptr->number);
}
