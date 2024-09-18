/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 11:51:44 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/17 18:12:55 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	forks_struct_init(t_data *data, t_fork **fork)
{
	int	i;

	i = 0;
	*fork = malloc(data->philo_nmemb * sizeof(t_fork));
	if (!(*fork))
		error_allocation_forks_struct_and_exit(data);
	while (i < data->philo_nmemb)
	{
		(*fork)[i].fork_id = i + 1;
		(*fork)[i].fork_is_available = true;
		if (pthread_mutex_init(&(*fork)[i].mutex_for_fork, NULL))
		{
			printf("here\n");
			error_fork_mutex_creation_and_exit(data, *fork, i);
		}
		i++;
	}
}

int	philos_struct_init(t_data *data, t_fork *fork, t_philo **philo)
{
	int	i;

	i = 0;
	*philo = malloc(data->philo_nmemb * sizeof(t_philo));
	if (!(*philo))
		error_allocation_philo_struct_and_exit(data, fork, data->philo_nmemb);
	while (i < data->philo_nmemb)
	{
		(*philo)[i].philo_id = i + 1;
		(*philo)[i].beginning_of_last_meal = 0;
		(*philo)[i].meals_count = 0;
		(*philo)[i].right_fork = &fork[i];
		(*philo)[i].left_fork = NULL;
		if (data->philo_nmemb > 1 && i != data->philo_nmemb - 1)
			(*philo)[i].left_fork = &fork[i + 1];
		else if (data->philo_nmemb > 1)
			(*philo)[i].left_fork = &fork[0];
		(*philo)[i].philo = philo;//to complete
		(*philo)[i].data = data;
		(*philo)[i].fork = fork;
		i++;
	}
	return (0);
}
