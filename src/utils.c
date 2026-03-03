/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopinha <sopinha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:00:00 by sopinha           #+#    #+#             */
/*   Updated: 2026/03/03 19:14:28 by sopinha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long milliseconds)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time();
	while (1)
	{
		elapsed = get_time() - start;
		if (elapsed >= milliseconds * 1000)
			break ;
		remaining = milliseconds * 1000 - elapsed;
		if (remaining > 5000)
			usleep(remaining / 2);
		else if (remaining > 1000)
			usleep(500);
		else
			usleep(100);
	}
}
