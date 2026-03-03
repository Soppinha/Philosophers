/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:27:34 by svaladar          #+#    #+#             */
/*   Updated: 2026/03/03 19:11:31 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_required_args(char **argv, t_data *data)
{
	if (!parse_int_arg(argv[1], &data->num_philos, ERR_NUM_PHILO))
		return (FALSE);
	if (!parse_long_arg(argv[2], &data->time_to_die, ERR_MS_DIE))
		return (FALSE);
	if (!parse_long_arg(argv[3], &data->time_to_eat, ERR_MS_EAT))
		return (FALSE);
	if (!parse_long_arg(argv[4], &data->time_to_sleep, ERR_MS_SLEEP))
		return (FALSE);
	return (TRUE);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		return (print_input_error(ERR_ARGS));
	if (!parse_required_args(argv, data))
		return (FALSE);
	data->num_must_eat = -1;
	if (argc == 6)
	{
		if (!parse_int_arg(argv[5], &data->num_must_eat, ERR_MS_MUST))
			return (FALSE);
	}
	return (TRUE);
}
