/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:09:09 by sofia             #+#    #+#             */
/*   Updated: 2026/03/16 18:09:02 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_usage(void)
{
	write(2, "Usage: ./philo [number_of_philosophers]", 38);
	write(2, " [time_to_die] [time_to_eat]", 28);
	write(2, " [time_to_sleep] [must_eat]\n", 28);
}

int	print_input_error(int option)
{
	if (option == ERR_ARGS)
		write(2, "Error: invalid number of arguments\n", 35);
	else if (option == ERR_NUM_PHILO)
		write(2, "Error: invalid number of philosophers\n", 38);
	else if (option == ERR_EMPTY)
		write(2, "Error: invalid empty argument\n", 30);
	else if (option == ERR_DIGIT)
		write(2, "Error: invalid argument: only digits accepted\n", 46);
	else if (option == ERR_OVERFLOW)
		write(2, "Error: invalid argument: value out of range\n", 44);
	else if (option == ERR_MEALS)
		write(2, "Error: invalid number of meals (minimum 1)\n", 43);
	print_usage();
	return (FALSE);
}

int	print_init_error(int option)
{
	if (option == ERR_MALLOC)
		write(2, "Error: memory allocation failed\n", 32);
	return (FALSE);
}
