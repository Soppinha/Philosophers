/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:43:20 by sofia             #+#    #+#             */
/*   Updated: 2026/03/16 18:09:31 by svaladar         ###   ########.fr       */
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
	printf("\033[31m %ld 🔹   ❌  philosopher %d dead ❌\033[0m\n",
		get_time() - ptr->rules->real_time, ptr->number);
}
