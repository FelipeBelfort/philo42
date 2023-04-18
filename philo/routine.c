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
		if (!*philo->dead)
		{
			*philo->dead = philo->id;
			printf("%llu %d died\n", timestamp() - philo->time_start, philo->id);
		}
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

// static int	check_break(t_philo *philo)
// {
// 	if (*philo->dead)
// 		return (1);
// 	if (timestamp() - philo->last_meal > philo->time_die)
// 	{
// 		pthread_mutex_lock(philo->dead_mutex);
// 		if (!*philo->dead)
// 		{
// 			*philo->dead = philo->id;
// 			printf("%llu %d died\n", timestamp() - philo->time_start, philo->id);
// 		}
// 		pthread_mutex_unlock(philo->dead_mutex);
// 		return (1);
// 	}
// 	return (0);
// }

int	get_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->test_mutex);
	if (philo->fork)
	{
		pthread_mutex_unlock(&philo->test_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->test_mutex);
	return (0);
}

void	set_fork(t_philo *philo, int fork)
{
	pthread_mutex_lock(&philo->test_mutex);
	philo->fork = fork;
	pthread_mutex_unlock(&philo->test_mutex);
}

static void	meal_time(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork_mutex);
	// pthread_mutex_lock(&philo->test_mutex);
	// philo->fork = 1;
	// pthread_mutex_unlock(&philo->test_mutex);
	set_fork(philo, 1);
	if (check_break(philo))
	{
		pthread_mutex_unlock(&philo->fork_mutex);
		return ;
	}
	printf("%llu %d has taken a fork\n", timestamp() - philo->time_start, philo->id);
	// pthread_mutex_lock(&philo->test_mutex);
	// while (get_fork(philo->next))
	// {
	// 	if (check_break(philo))
	// 	{
	// 		// pthread_mutex_unlock(&philo->test_mutex);
	// 		pthread_mutex_unlock(&philo->fork_mutex);
	// 		return ;
	// 	}
	// 	usleep(250);
	// }
	// pthread_mutex_unlock(&philo->test_mutex);
	pthread_mutex_lock(&philo->next->fork_mutex);
	// pthread_mutex_lock(&philo->next->test_mutex);
	// philo->next->fork = 1;
	// pthread_mutex_unlock(&philo->next->test_mutex);
	set_fork(philo->next, 1);
	if (check_break(philo))
	{
		pthread_mutex_unlock(&philo->fork_mutex);
		pthread_mutex_unlock(&philo->next->fork_mutex);
		return ;
	}
	philo->last_meal = timestamp();
	printf("%llu %d is eating\n", philo->last_meal - philo->time_start, philo->id);
	philo->meals++;
	while (timestamp() < philo->last_meal + philo->time_eat)
		usleep(250);
	// pthread_mutex_lock(&philo->test_mutex);	
	// philo->fork = 0;
	// pthread_mutex_unlock(&philo->test_mutex);
	// pthread_mutex_lock(&philo->next->test_mutex);
	// philo->next->fork = 0;
	// pthread_mutex_unlock(&philo->next->test_mutex);
	set_fork(philo, 0);
	set_fork(philo->next, 0);
	pthread_mutex_unlock(&philo->fork_mutex);
	pthread_mutex_unlock(&philo->next->fork_mutex);

}

// static void	meal_time(t_philo *philo)
// {
// 	pthread_mutex_lock(&philo->fork_mutex);
// 	philo->fork = 1;
// 	if (check_break(philo))
// 	{
// 		pthread_mutex_unlock(&philo->fork_mutex);
// 		return ;
// 	}
// 	printf("%llu %d has taken a fork\n", timestamp() - philo->time_start, philo->id);
// 	while (philo->next->fork)
// 	{
// 		if (check_break(philo))
// 		{
// 			pthread_mutex_unlock(&philo->fork_mutex);
// 			return ;
// 		}
// 		usleep(250);
// 	}
// 	pthread_mutex_lock(&philo->next->fork_mutex);
// 	philo->next->fork = 1;
// 	if (check_break(philo))
// 	{
// 		pthread_mutex_unlock(&philo->fork_mutex);
// 		pthread_mutex_unlock(&philo->next->fork_mutex);
// 		return ;
// 	}
// 	philo->last_meal = timestamp();
// 	printf("%llu %d is eating\n", philo->last_meal - philo->time_start, philo->id);
// 	philo->meals++;
// 	while (timestamp() < philo->last_meal + philo->time_eat)
// 		usleep(250);
// 	philo->fork = 0;
// 	philo->next->fork = 0;
// 	pthread_mutex_unlock(&philo->fork_mutex);
// 	pthread_mutex_unlock(&philo->next->fork_mutex);

// }

static int	thinking_time(t_philo *philo)
{
	if (get_fork(philo) || get_fork(philo->next))
		printf("%llu %d is thinking\n", timestamp() - philo->time_start, philo->id);
	while (get_fork(philo))
	{
		if (check_break(philo))
			return (1);
		usleep(500);
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
		usleep(250);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	pthread_mutex_lock(philo->dead_mutex);
	pthread_mutex_unlock(philo->dead_mutex);
	// pthread_mutex_lock(&philo->fork_mutex);
	// pthread_mutex_unlock(&philo->fork_mutex);
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
