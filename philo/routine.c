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

static int	check_break(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	if (timestamp() - philo->last_meal > philo->time_die)
	{
		// if (!*philo->dead)
		// {
		// }
		*philo->dead = philo->id;
		printf("%llu %d died\n", timestamp() - philo->time_start, philo->id);
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

static void	meal_time(t_philo *ph)
{
	set_fork(ph, 1);
	if (check_break(ph))
		return (set_fork(ph, 0));
	printf("%llu %d has taken a fork\n", timestamp() - ph->time_start, ph->id);
	while (get_fork(ph->next))
	{
		if (check_break(ph))
			return ;
		usleep(250);
	}
	set_fork(ph->next, 1);
	if (check_break(ph))
	{
		set_fork(ph, 0);
		set_fork(ph->next, 0);
		return ;
	}
	ph->last_meal = timestamp();
	printf("%llu %d is eating\n", ph->last_meal - ph->time_start, ph->id);
	ph->meals++;
	while (timestamp() < ph->last_meal + ph->time_eat)
		usleep(250);
	set_fork(ph, 0);
	set_fork(ph->next, 0);
}

static int	thinking_time(t_philo *ph)
{
	if (get_fork(ph) || get_fork(ph->next))
		printf("%llu %d is thinking\n", timestamp() - ph->time_start, ph->id);
	while (get_fork(ph))
	{
		if (check_break(ph))
			return (1);
		usleep(250);
	}
	return (0);
}

static int	sleeping_time(t_philo *philo)
{
	t_stmp	go_sleep;

	if (check_break(philo))
		return (1);
	go_sleep = timestamp();
	printf("%llu %d is sleeping\n", go_sleep - philo->time_start, philo->id);
	while (timestamp() < go_sleep + philo->time_sleep)
	{
		if (check_break(philo))
			return (1);
		usleep(250);
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(philo->dead_mutex);
	pthread_mutex_unlock(philo->dead_mutex);
	while (timestamp() < philo->time_start)
		usleep(200);
	if (philo->id % 2 == 0)
		usleep(500);
	while (!check_break(philo))
	{
		if (philo->max_meal && philo->meals >= philo->max_meal)
			break ;
		if (thinking_time(philo))
			break ;
		meal_time(philo);
		if (sleeping_time(philo))
			break ;
	}
	return (NULL);
}
