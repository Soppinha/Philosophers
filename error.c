/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:40:46 by svaladar          #+#    #+#             */
/*   Updated: 2026/01/13 20:44:14 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_ac_invalid(void)
{
	printf("./philo [number_of_philosophers] [time_to_die] "
		"[time_to_eat] [time_to_sleep] "
		"optional [number_of_times_each_philosopher_must_eat]\n");
}

void	error_n_philo(void)
{
	printf("You need to have at least one philosopher");
}
