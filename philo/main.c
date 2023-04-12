/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 13:45:38 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/12 15:10:54 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_args_type(char **argv)
{
	int	i;
	int	j;

	i = 0;
	while (argv[++i])
	{
		j = -1;
		while (argv[i][++j])
		{
			if (!ft_isdigit(argv[i][j]))
				return (1);
		}
	}
	return (0);
}

void	handle_exception_cases(char **argv)
{
	(void) argv;
	printf("todo.\n");
}

int	check_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error, wrong amount of arguments. Expected blabla");
		return (1);
	}
	if (check_args_type(argv))
	{
		printf("The arguments have to be numeric");
		return (1);
	}
	if (!ft_atoli(argv[1]) || ft_atoli(argv[1]) == 1 || (argv[5] && !ft_atoli(argv[5])))
	{
		handle_exception_cases(argv);
		return (1);
	}
	return (0);
}

/**
 * manter o parsing (se estiver bom)
 * trocar o t_philo para uma lista circular (verificar para caso de só um philo)
 * add someone_dead
 * pensar em todas as possibilidades de morte para poder testar corretamente
*/

pthread_mutex_t	*init_mutex(char *arg)
{
	pthread_mutex_t	*forks;
	size_t			philos;

	philos = ft_atoli(arg);
	forks = ft_calloc(philos + 1, sizeof(*forks));
	if (!forks)
		return (NULL);
	while (--philos >= 0)
		pthread_mutex_init(&forks[philos], NULL);
	return (forks);
}

t_philo	**init_philos(char **argv, pthread_mutex_t *forks)
{
	t_philo	**philos;
	size_t	philo_nb;

	philo_nb = ft_atoli(argv[1]);
	philos = ft_calloc(philo_nb + 1, sizeof(t_philo *));
	if (!philos)
		return (NULL);
	philos[--philo_nb] = ft_calloc(1, sizeof(t_philo));
	if (!philos[philo_nb])
		return (NULL);
	philos[philo_nb]->lf = &forks[philo_nb];
	philos[philo_nb]->rf = &forks[0];
	philos[philo_nb]->id = philo_nb + 1;
	philos[philo_nb]->time_die = ft_atoli(argv[2]);
	philos[philo_nb]->time_eat = ft_atoli(argv[3]);
	philos[philo_nb]->time_sleep = ft_atoli(argv[4]);
	if (argv[5])
		philos[philo_nb]->max_lunch = ft_atoli(argv[5]);
	while (--philo_nb >= 0)
	{
		philos[philo_nb] = ft_calloc(1, sizeof(t_philo));
		if (!philos[philo_nb])
			return (NULL);
		philos[philo_nb]->lf = &forks[philo_nb];
		philos[philo_nb]->rf = &forks[philo_nb + 1];
		philos[philo_nb]->id = philo_nb + 1;
		philos[philo_nb]->time_die = philos[philo_nb + 1]->time_die;
		philos[philo_nb]->time_eat = philos[philo_nb + 1]->time_eat;
		philos[philo_nb]->time_sleep = philos[philo_nb + 1]->time_sleep;
		philos[philo_nb]->max_lunch = philos[philo_nb + 1]->max_lunch;
	}
	return (philos);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	if (philo->id % 2 == 0)
	{
		printf("%d is thinking\n", philo->id);
		usleep(philo->time_eat);
	}
	while (1)
	{
		pthread_mutex_lock(philo->lf);
		pthread_mutex_lock(philo->rf);
		printf("%d has taken a fork\n", philo->id);
		printf("%d is eating\n", philo->id);
		philo->lunch++;
		usleep(philo->time_eat);
		pthread_mutex_unlock(philo->lf);
		pthread_mutex_unlock(philo->rf);
		printf("%d is sleeping\n", philo->id);
		usleep(philo->time_sleep);
		printf("%d is thinking\n", philo->id);
		if (philo->lunch == philo->max_lunch)
			break ;
	}
	return (NULL);
}

void	init_threads(t_philo **philos)
{
	int	i;

	i = -1;
	while (philos[++i])
	{
		pthread_create(&philos[i]->philo, NULL, &routine, philos[i]);
	}
	while (--i >= 0)
		pthread_join(philos[i]->philo, NULL);
}

int	main(int argc, char **argv)
{
	pthread_mutex_t	*forks;
	t_philo			**philos;

	if (check_args(argc, argv))
		return (1);
	forks = init_mutex(argv[1]);
	if (!forks)
		return (1);
	philos = init_philos(argv, forks);
	if (!philos)
		return (1);
	init_threads(philos);
	return (0);
}
