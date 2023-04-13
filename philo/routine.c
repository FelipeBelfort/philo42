/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 14:07:02 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/13 19:26:48 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_break(t_philo *philo)
{
	if ((philo->max_meal && philo->meals == philo->max_meal) || *philo->dead)
		return (1);
	if (timestamp() - philo->last_meal > philo->time_die)
	{
		pthread_mutex_lock(philo->dead_mutex);
		if (!*philo->dead)
		{
			printf("%llu %d died\n", timestamp(), philo->id);
			*philo->dead = philo->id;
		}
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	return (0);
}

static void	meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_mutex);
	philo->fork = 1;
	printf("%llu %d has taken a fork\n", timestamp(), philo->id);
	while (philo->next->fork)
	{
		if (check_break(philo))
			return ;
		usleep(100);
	}
	pthread_mutex_lock(&philo->next->fork_mutex);
	philo->next->fork = 1;
	printf("%llu %d is eating\n", timestamp(), philo->id);
	philo->last_meal = timestamp();
	philo->meals++;
	usleep(philo->time_eat * 1000);
	philo->fork = 0;
	pthread_mutex_unlock(&philo->fork_mutex);
	philo->next->fork = 0;
	pthread_mutex_unlock(&philo->next->fork_mutex);

}

static int	thinking_time(t_philo *philo)
{
	if (check_break(philo))
		return (1);
	if (philo->fork)
		printf("%llu %d is thinking\n", timestamp(), philo->id);
	while (philo->fork)
	{
		if (check_break(philo))
			return (1);
		usleep(500);
	}
	return (0);
}

static int	sleeping_time(t_philo *philo)
{

	if (check_break(philo))
		return (1);
	printf("%llu %d is sleeping\n", timestamp(), philo->id);
	usleep(philo->time_sleep * 1000);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	// pthread_mutex_lock(philo->dead_mutex);
	// pthread_mutex_unlock(philo->dead_mutex);
	while (*philo->dead)
		;
	philo->last_meal = timestamp();
	if (philo->id % 2 == 0)
		usleep(250);
	while (1)
	{
		if (thinking_time(philo))
			break ;
		meal_time(philo);
		if (sleeping_time(philo))
			break ;
	}
	return (NULL);
}
