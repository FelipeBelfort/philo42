/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: FelipeBelfort <FelipeBelfort@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 16:01:12 by fbelfort          #+#    #+#             */
/*   Updated: 2023/04/13 19:57:36 by FelipeBelfo      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

int	ft_atoi(const char *str)
{
	int				i;
	long long int	nb;
	long long int	minus;

	i = 0;
	nb = 0;
	minus = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			minus = -1;
	if (str[i] == '-' || str[i] == '+')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		nb = nb * 10 + str[i++] - '0';
	nb *= minus;
	if (nb == (int) nb)
		return (nb);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size && count > 4294967295 / size)
		return (NULL);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, count * size);
	return (ptr);
}

static int	check_args_type(char **argv)
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

int	check_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: Expected 'number_of_philosophers ");
		printf("time_to_die time_to_eat");
		printf(" time_to_sleep [number_of_times_each_philosopher_must_eat]'\n");
		return (1);
	}
	if (check_args_type(argv))
	{
		printf("The arguments have to be numeric and positive\n");
		return (1);
	}
	if (!ft_atoi(argv[1]) || (argv[5] && !ft_atoi(argv[5])))
	{
		handle_exception_cases(argv);
		return (1);
	}
	return (0);
}
