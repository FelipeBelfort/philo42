/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 14:07:02 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/19 01:50:49 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief
 * It will verify if the fork of the given philosopher is 0(free) or 1(taken)
 * then return the result
 * @param t_philo* philo -> the philosopher we want to verify the fork
 * @return
 * the state of the fork
 * 0 -> free or 1 -> taken
*/
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

/**
 *  @brief
 * It will set the fork value of the given philosopher to the fork argument
 * @param t_philo* philo -> the philosopher we want to set the fork
 * @param int fork -> the value to be set 0 -> free or 1 -> taken
*/
void	set_fork(t_philo *philo, int fork)
{
	pthread_mutex_lock(&philo->fork_mutex);
	philo->fork = fork;
	pthread_mutex_unlock(&philo->fork_mutex);
}
