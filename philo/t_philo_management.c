/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_philo_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 23:07:18 by FelipeBelfo       #+#    #+#             */
/*   Updated: 2023/04/13 19:50:43 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_clearlst(t_philo **lst)
{
	t_philo	*tmp;
	t_philo	*next;

	if (!lst || !(*lst))
		return ;
	tmp = *lst;
	while (tmp && tmp->next != *lst)
	{
		next = tmp->next;
		pthread_mutex_destroy(&tmp->fork_mutex);
		pthread_mutex_destroy(&tmp->test_mutex);
		free(tmp);
		tmp = NULL;
		tmp = next;
	}
	pthread_mutex_destroy(&tmp->fork_mutex);
	free(tmp);
	tmp = NULL;
}

t_philo	*philo_new(int id)
{
	t_philo	*node;

	node = ft_calloc(1, sizeof(*node));
	if (!node)
		return (NULL);
	node->id = id;
	pthread_mutex_init(&node->fork_mutex, NULL);
	pthread_mutex_init(&node->test_mutex, NULL);
	node->next = node;
	return (node);
}

t_philo	*philo_getlast(t_philo *lst)
{
	t_philo	*node;

	if (!lst)
		return (NULL);
	node = lst;
	while (node->next && node->next != lst)
		node = node->next;
	return (node);
}

int	philo_addback(t_philo **lst, t_philo *node)
{
	if (!node)
	{
		philo_clearlst(lst);
		return (1);
	}
	if (!lst)
		lst = &node;
	if (!(*lst))
		*lst = node;
	else
		philo_getlast(*lst)->next = node;
	node->next = *lst;
	return (0);
}

void	philo_update(t_philo *lst, pthread_mutex_t *dead_mutex)
{
	t_philo			*tmp;

	lst->dead_mutex = dead_mutex;
	tmp = lst->next;
	while (tmp != lst)
	{
		tmp->dead_mutex = dead_mutex;
		tmp->dead = lst->dead;
		tmp->max_meal = lst->max_meal;
		tmp->time_die = lst->time_die;
		tmp->time_eat = lst->time_eat;
		tmp->time_sleep = lst->time_sleep;
		tmp = tmp->next;
	}
}
