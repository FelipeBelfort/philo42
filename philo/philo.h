/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 13:40:14 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/14 14:26:42 by FelipeBelfo      ###   ########.fr       */
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

typedef unsigned long long int	t_stmp;

typedef struct s_philo
{
	int				id;
	int				*dead;
	int				meals;
	int				max_meal;
	t_stmp			last_meal;
	t_stmp			time_eat;
	t_stmp			time_sleep;
	t_stmp			time_die;
	t_stmp			time_start;
	pthread_t		philo;
	pthread_mutex_t	fork_mutex;
	int				fork;
	pthread_mutex_t	*dead_mutex;
	struct s_philo	*next;

}	t_philo;

/* args parsing */

int		ft_atoi(const char *str);
int		ft_isdigit(int c);
void	*ft_calloc(size_t count, size_t size);
int		check_args(int argc, char **argv);
void	handle_exception_cases(char **argv);

/* t_philo management */

void	philo_clearlst(t_philo **lst);
t_philo	*philo_new(int id);
t_philo	*philo_getlast(t_philo *lst);
int		philo_addback(t_philo **lst, t_philo *node);
void	philo_update(t_philo *lst, pthread_mutex_t *dead_mutex);

/* threads routine */

void	*routine(void *arg);
int		get_fork(t_philo *philo);
void	set_fork(t_philo *philo, int fork);

/* timestamp */

t_stmp	timestamp(void);
t_stmp	time_diff(t_stmp diff);

# define FORK "%llu %d has taken a fork\n"
# define EAT "%llu %d is eating\n"
# define THINK "%llu %d is thinking\n"
# define SLEEP "%llu %d is sleeping\n"
# define DEAD "%llu %d died\n"

#endif
