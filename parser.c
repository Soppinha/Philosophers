/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wedos-sa <wedos-sa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 11:03:10 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/12/19 14:20:57 by wedos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	err(const char *msg)
{
	while (*msg)
		write(2, msg++, 1);
	exit(EXIT_FAILURE);
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
*/
int	valid_input(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '\0')
			err("ERROR: Empty argument.\n");
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				err("ERROR: Only digits are accepted.\n");
			j++;
		}
		if (is_overflow(argv[i]))
			err("ERROR: Value out of range.\n");
		i++;
	}
	return (1);
}

void	print_error(char **argv)
{
	if (ft_atoi(argv[1]) < 1)
		err("ERROR: Minimum 1 philosopher.\n");
	err("ERROR: Try: ./philo 5 800 200 200\n");
}

void	init_check(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		err("ERROR: Philosophers need 4 or 5 arguments.\n");
	valid_input(argv);
	if (ft_atoi(argv[1]) < 1)
		print_error(argv);
	if (argc == 6 && ft_atoi(argv[5]) < 1)
		err("ERROR: Minimum 1 meal.\n");
}
