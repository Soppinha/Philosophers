/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:27:22 by svaladar          #+#    #+#             */
/*   Updated: 2026/03/03 19:45:13 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	philos = NULL;
	if (parse_arguments(argc, argv, &data) == FALSE)
		return (1);
	if (init_data(&data, &philos) == FALSE)
	{
		cleanup(&data, philos);
		return (1);
	}
	if (start_simulation(&data, philos) == FALSE)
	{
		cleanup(&data, philos);
		return (1);
	}
	cleanup(&data, philos);
	return (0);
}
