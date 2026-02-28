/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 18:45:31 by sopinha           #+#    #+#             */
/*   Updated: 2026/02/28 18:38:37 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
		return (FALSE);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	validate_and_convert(char *arg, int *value, int err_code)
{
	long	num;

	if (!is_valid_number(arg))
		return (error_input_msg(err_code));
	num = ft_unsigned_atol(arg);
	if (num <= 0 || num > INT_MAX)
		return (error_input_msg(err_code));
	*value = (int)num;
	return (TRUE);
}

int	validate_and_convert_long(char *arg, long *value, int err_code)
{
	long	num;

	if (!is_valid_number(arg))
		return (error_input_msg(err_code));
	num = ft_unsigned_atol(arg);
	if (num <= 0 || num > INT_MAX)
		return (error_input_msg(err_code));
	*value = num;
	return (TRUE);
}
