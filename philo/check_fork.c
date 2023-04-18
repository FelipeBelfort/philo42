/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 14:07:02 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/14 16:21:13 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_mutex);
	if (philo->fork)
	{
		pthread_mutex_unlock(&philo->fork_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->fork_mutex);
	return (0);
}

void	set_fork(t_philo *philo, int fork)
{
	pthread_mutex_lock(&philo->fork_mutex);
	philo->fork = fork;
	pthread_mutex_unlock(&philo->fork_mutex);
}
