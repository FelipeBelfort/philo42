/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 13:40:14 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/10 23:52:35 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo
{
	int				id;
	int				dead;
	int				lunch;
	int				max_lunch;
	int				time_eat;
	int				time_sleep;
	int				time_die;
	pthread_t		philo;
	pthread_mutex_t	*lf;
	pthread_mutex_t	*rf;

}	t_philo;

long long int	ft_atoli(const char *str);
int				ft_isdigit(int c);
void			*ft_calloc(size_t count, size_t size);

#endif
