/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sofia <sofia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:10 by wedos-sa          #+#    #+#             */
/*   Updated: 2026/03/04 17:14:49 by sofia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static int	is_overflow(const char *s)
{
	const char	*max = "2147483647";
	int			len;

	len = ft_strlen(s);
	if (len > 10)
		return (1);
	if (len < 10)
		return (0);
	while (*s)
	{
		if (*s > *max)
			return (1);
		if (*s < *max)
			return (0);
		s++;
		max++;
	}
	return (0);
}

int	valid_input(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '\0')
			return (print_input_error(ERR_EMPTY));
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (print_input_error(ERR_DIGIT));
			j++;
		}
		if (is_overflow(argv[i]))
			return (print_input_error(ERR_OVERFLOW));
		i++;
	}
	return (1);
}

int	init_check(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (print_input_error(ERR_ARGS));
	if (!valid_input(argv))
		return (0);
	if (ft_atoi(argv[1]) < 1)
		return (print_input_error(ERR_NUM_PHILO));
	if (argc == 6 && ft_atoi(argv[5]) < 1)
		return (print_input_error(ERR_MEALS));
	return (1);
}