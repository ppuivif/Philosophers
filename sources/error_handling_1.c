/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:09:20 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 15:15:33 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_allocation_forks_struct_and_exit(t_data *data)
{
	ft_putstr_fd("error : a fork allocation failed\n", 2);
	pthread_mutex_destroy(&data->mutex_for_print);
	pthread_mutex_destroy(&data->mutex_for_data_access);
	exit(EXIT_FAILURE);
}

void	error_fork_mutex_creation_and_exit(t_data *data, t_fork *fork, \
int nmemb)
{
	ft_putstr_fd("error : fork mutex creation failed\n", 2);
	all_mutex_destroy(data, fork, nmemb);
	if (fork)
		free(fork);
	exit(EXIT_FAILURE);
}

void	error_allocation_philo_struct_and_exit(t_data *data, t_fork *fork, \
int nmemb)
{
	ft_putstr_fd("error : a philo allocation failed\n", 2);
	all_mutex_destroy(data, fork, nmemb);
	if (fork)
		free(fork);
	exit(EXIT_FAILURE);
}

void	error_philo_thread_creation(t_data *data, t_fork *fork, t_philo *philo, \
int nmemb)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->mutex_for_print);
	ft_putstr_fd("error : a thread creation failed\n", 2);
	pthread_mutex_unlock(&data->mutex_for_print);
	while (i < nmemb)
	{
		if (pthread_detach(philo[i].philo_thread))
			error_philo_thread_detach(data, fork, philo);
		i++;
	}
	mutex_destroy_and_free_all(data, fork, philo);
	exit(EXIT_FAILURE);
}
