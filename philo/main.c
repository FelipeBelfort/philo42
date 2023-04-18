/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 13:45:38 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/14 16:23:38 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_exception_cases(char **argv)
{
	int	philo_nb;
	int	meals_nb;
	int	i;

	i = 0;
	philo_nb = ft_atoi(argv[1]);
	meals_nb = 1;
	if (!philo_nb)
		printf("Philosopher error: no philosopher\n");
	if (argv[5])
		meals_nb = ft_atoi(argv[5]);
	if (!meals_nb)
		while (++i <= philo_nb)
			printf("%llu %d has taken a fork\n", timestamp(), i);
}

t_stmp	timestamp(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return ((timestamp.tv_sec * 1000) + (timestamp.tv_usec / 1000));
}

static t_philo	*init_philos(char **av, pthread_mutex_t *dead_mutex, int *dead)
{
	t_philo	*philos;
	t_philo	*tmp;
	size_t	philo_nb;
	size_t	i;

	i = 1;
	philo_nb = ft_atoi(av[1]);
	philos = philo_new(1);
	if (!philos)
		return (NULL);
	philos->time_die = ft_atoi(av[2]);
	philos->time_eat = ft_atoi(av[3]);
	philos->time_sleep = ft_atoi(av[4]);
	if (av[5])
		philos->max_meal = ft_atoi(av[5]);
	philos->dead = dead;
	while (++i <= philo_nb)
	{
		tmp = philo_new(i);
		if (philo_addback(&philos, tmp))
			return (NULL);
	}
	philo_update(philos, dead_mutex);
	return (philos);
}

static void	init_threads(t_philo *philos)
{
	int		i;
	t_stmp	start;

	i = 0;
	// *philos->dead = 1;
	pthread_mutex_lock(philos->dead_mutex);
	while (philos->id > i)
	{
		// pthread_mutex_lock(&philos->fork_mutex);
		if (pthread_create(&philos->philo, NULL, &routine, philos))
			return (perror("Philosophers error: "));
		philos = philos->next;
		i++;
	}
	start = timestamp() + (i * 50);
	while (!philos->time_start)
	{
		philos->time_start = start;
		philos->last_meal = start;
		// pthread_mutex_unlock(&philos->fork_mutex);
		philos = philos->next;
	}
	pthread_mutex_unlock(philos->dead_mutex);
	// *philos->dead = 0;
	while (i)
	{
		pthread_join(philos->philo, NULL);
		philos = philos->next;
		i--;
	}
}

int	main(int argc, char **argv)
{
	t_philo			*philos;
	pthread_mutex_t	dead_mutex;
	int				dead;

	dead = 0;
	if (check_args(argc, argv))
		return (1);
	philos = init_philos(argv, &dead_mutex, &dead);
	if (!philos)
		return (1);
	pthread_mutex_init(&dead_mutex, NULL);
	init_threads(philos);
	pthread_mutex_destroy(&dead_mutex);
	philo_clearlst(&philos);
	return (0);
}
