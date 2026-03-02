/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 18:45:31 by sopinha           #+#    #+#             */
/*   Updated: 2026/03/02 18:22:14 by sopinha          ###   ########.fr       */
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

static int	convert_value(char *arg, long *out, int err_code)
{
    long	num;

    if (!is_valid_number(arg))
        return (error_input_msg(err_code));
    num = ft_unsigned_atol(arg);
    if (num <= 0 || num > INT_MAX)
        return (error_input_msg(err_code));
    *out = num;
    return (TRUE);
}

int	validate_and_convert(char *arg, int *value, int err_code)
{
    long	tmp;

    if (!convert_value(arg, &tmp, err_code))
        return (FALSE);
    *value = (int)tmp;
    return (TRUE);
}

int	validate_and_convert_long(char *arg, long *value, int err_code)
{
    return (convert_value(arg, value, err_code));
}
