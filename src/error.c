/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 20:40:46 by svaladar          #+#    #+#             */
/*   Updated: 2026/03/03 19:10:55 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_usage(void)
{
	ft_putstr_fd("Usage: ./philo [number_of_philosophers] ", 2);
	ft_putstr_fd("[time_to_die] [time_to_eat] [time_to_sleep] ", 2);
	ft_putstr_fd("[number_of_times_each_philosopher_must_eat]\n", 2);
}

int	print_input_error(int option)
{
	if (option == ERR_ARGS)
	{
		ft_putstr_fd("Error: invalid number of arguments\n", 2);
		print_usage();
	}
	else if (option == ERR_NUM_PHILO)
		ft_putstr_fd("Error: invalid number of philosophers\n", 2);
	else if (option == ERR_MS_DIE)
		ft_putstr_fd("Error: invalid time_to_die\n", 2);
	else if (option == ERR_MS_EAT)
		ft_putstr_fd("Error: invalid time_to_eat\n", 2);
	else if (option == ERR_MS_SLEEP)
		ft_putstr_fd("Error: invalid time_to_sleep\n", 2);
	else if (option == ERR_MS_MUST)
		ft_putstr_fd("Error: invalid must_eat value\n", 2);
	else if (option == ERR_NUM)
		ft_putstr_fd("Error: invalid number\n", 2);
	return (FALSE);
}

int	print_init_error(int option)
{
	if (option == ERR_MALLOC)
		ft_putstr_fd("Error: memory allocation failed\n", 2);
	else if (option == ERR_MUTEX)
		ft_putstr_fd("Error: mutex initialization failed\n", 2);
	else if (option == ERR_THREAD)
		ft_putstr_fd("Error: thread creation failed\n", 2);
	return (FALSE);
}

int	print_time_error(int option)
{
	if (option == ERR_TIME)
		ft_putstr_fd("Error: time retrieval failed\n", 2);
	return (FALSE);
}
