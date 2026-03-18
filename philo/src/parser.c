/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 18:09:51 by sofia             #+#    #+#             */
/*   Updated: 2026/03/16 18:09:42 by svaladar         ###   ########.fr       */
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
		return (TRUE);
	if (len < 10)
		return (FALSE);
	while (*s)
	{
		if (*s > *max)
			return (TRUE);
		if (*s < *max)
			return (FALSE);
		s++;
		max++;
	}
	return (FALSE);
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
	return (TRUE);
}

int	init_check(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (print_input_error(ERR_ARGS));
	if (!valid_input(argv))
		return (FALSE);
	if (ft_atoi(argv[1]) < 1)
		return (print_input_error(ERR_NUM_PHILO));
	if (argc == 6 && ft_atoi(argv[5]) < 1)
		return (print_input_error(ERR_MEALS));
	return (TRUE);
}
