/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:27:34 by svaladar          #+#    #+#             */
/*   Updated: 2026/02/27 05:13:45 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	parse_required(char **argv, t_data *data)
{
	if (!validate_and_convert(argv[1], &data->num_philos, ERR_NUM_PHILO))
		return (FALSE);
	if (!validate_and_convert_long(argv[2], &data->time_to_die, ERR_MS_DIE))
		return (FALSE);
	if (!validate_and_convert_long(argv[3], &data->time_to_eat, ERR_MS_EAT))
		return (FALSE);
	if (!validate_and_convert_long(argv[4], &data->time_to_sleep, ERR_MS_SLEEP))
		return (FALSE);
	return (TRUE);
}

int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
		return (error_input_msg(ERR_ARGS));
	if (!parse_required(argv, data))
		return (FALSE);
	data->num_must_eat = -1;
	if (argc == 6)
	{
		if (!validate_and_convert(argv[5], &data->num_must_eat, ERR_MS_MUST))
			return (FALSE);
	}
	return (TRUE);
}
