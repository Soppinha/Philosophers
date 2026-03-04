/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:10 by wedos-sa          #+#    #+#             */
/*   Updated: 2026/03/04 00:07:29 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	err(const char *msg)
{
	while (*msg)
		write(2, msg++, 1);
	return (0);
}

static int	ft_strlen_local(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

/*
** Returns 1 if the numeric string exceeds INT_MAX (2147483647).
*/
static int	is_overflow(const char *s)
{
	const char	*max = "2147483647";
	int			len;

	len = ft_strlen_local(s);
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

/*
** Rejects empty strings and any argument that contains a non-digit character.
** This catches '-', '+', letters and any other symbol including '-5'.
** Returns 1 on valid input, 0 on error.
*/
int	valid_input(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '\0')
			return (err("ERROR: Invalid argument: empty string.\n"));
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (err("ERROR: Invalid argument: only digits accepted.\n"));
			j++;
		}
		if (is_overflow(argv[i]))
			return (err("ERROR: Invalid argument: value out of range.\n"));
		i++;
	}
	return (1);
}

static int	print_error(char **argv)
{
	if (ft_atoi(argv[1]) < 1)
		return (err("ERROR: Invalid number of philosophers (minimum 1).\n"));
	return (err("ERROR: Invalid arguments. Try: ./philo 5 800 200 200\n"));
}

int	init_check(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (err("ERROR: Invalid number of arguments (need 4 or 5).\n"));
	if (!valid_input(argv))
		return (0);
	if (ft_atoi(argv[1]) < 1)
		return (print_error(argv));
	if (argc == 6 && ft_atoi(argv[5]) < 1)
		return (err("ERROR: Invalid number of meals (minimum 1).\n"));
	return (1);
}