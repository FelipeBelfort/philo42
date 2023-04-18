/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 14:07:02 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/19 01:50:51 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief
 * Check if the given philosopher should die or
 * if another philosopher is already dead.
 * If one of the conditions is true,
 * prints the message if that's the case and returns 1
 * otherwise returns 0
*/
static int	check_death(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_mutex);
	if (*philo->dead)
	{
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	if (timestamp() - philo->last_meal > philo->time_die)
	{
		*philo->dead = philo->id;
		printf("%llu %d died\n", timestamp() - philo->time_start, philo->id);
		pthread_mutex_unlock(philo->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(philo->dead_mutex);
	return (0);
}

/**
 * @brief
 * it will take his own fork, print the fork message,
 *  wait for the next fork to be free, take it to himself
 *  print the second fork message and the eating message,
 * increment the quantity of meals and free the forks
*/
static void	meal_time(t_philo *ph)
{
	set_fork(ph, 1);
	if (check_death(ph))
		return (set_fork(ph, 0));
	printf("%llu %d has taken a fork\n", timestamp() - ph->time_start, ph->id);
	while (get_fork(ph->next))
	{
		if (check_death(ph))
			return ;
		usleep(250);
	}
	set_fork(ph->next, 1);
	if (check_death(ph))
	{
		set_fork(ph, 0);
		set_fork(ph->next, 0);
		return ;
	}
	ph->last_meal = timestamp();
	printf("%llu %d has taken a fork\n", ph->last_meal - ph->time_start, ph->id);
	printf("%llu %d is eating\n", ph->last_meal - ph->time_start, ph->id);
	ph->meals++;
	while (timestamp() < ph->last_meal + ph->time_eat)
		usleep(250);
	set_fork(ph, 0);
	set_fork(ph->next, 0);
}

/**
 * @brief
 * if at least one fork is already taken it prints the thinking message
 * and waits untill the fork is freed meanwhile checking the death
 * @return
 * 1 = someone is dead or
 * 0 = everything is ok
*/
static int	thinking_time(t_philo *ph)
{
	if (get_fork(ph) || get_fork(ph->next))
		printf("%llu %d is thinking\n", timestamp() - ph->time_start, ph->id);
	while (get_fork(ph))
	{
		if (check_death(ph))
			return (1);
		usleep(250);
	}
	return (0);
}

/**
 * @brief
 * If everyone is alive print the sleeping message
 * and waits untill it's over checking the death meanwhile
 * @return
 * 1 = someone is dead or
 * 0 = everything is ok
*/
static int	sleeping_time(t_philo *philo)
{
	t_stmp	go_sleep;

	if (check_death(philo))
		return (1);
	go_sleep = timestamp();
	printf("%llu %d is sleeping\n", go_sleep - philo->time_start, philo->id);
	while (timestamp() < go_sleep + philo->time_sleep)
	{
		if (check_death(philo))
			return (1);
		usleep(250);
	}
	return (0);
}

/**
 * @brief
 * lock and unlock the dead_mutex the waits for the time_start
 * forcing all the threads to start together
 * make the even philosophers wait to avoid conflicts taking the forks
 * at the beginning.
 * After all that it enters in a loop of thinking, eating and sleeping
 * untill someone is dead or the philosopher ate all his meals
*/
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
	while (!check_death(philo))
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
