/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svaladar <svaladar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 18:32:32 by svaladar          #+#    #+#             */
/*   Updated: 2026/01/13 20:44:26 by svaladar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>

typedef struct s_philo
{
	char	name;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_think;
}	t_philo;

void	error_ac_invalid(void);
void	error_n_philo(void);

#endif