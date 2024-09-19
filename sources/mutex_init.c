/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 09:57:37 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 15:20:21 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	data_mutex_init_1(t_data *data)
{
	if (pthread_mutex_init(&data->mutex_for_print, NULL))
	{
		ft_putstr_fd("error : a mutex creation failed\n", 2);
		exit(EXIT_FAILURE);
	}
	if (pthread_mutex_init(&data->mutex_for_data_access, NULL))
	{
		ft_putstr_fd("error : a mutex creation failed\n", 2);
		pthread_mutex_destroy(&data->mutex_for_print);
		exit(EXIT_FAILURE);
	}
}

void	data_mutex_init_2(t_data *data)
{
	if (pthread_mutex_init(&data->mutex_for_stop, NULL))
	{
		ft_putstr_fd("error : a mutex creation failed\n", 2);
		pthread_mutex_destroy(&data->mutex_for_print);
		pthread_mutex_destroy(&data->mutex_for_data_access);
		exit(EXIT_FAILURE);
	}
	if (pthread_mutex_init(&data->mutex_for_death, NULL))
	{
		ft_putstr_fd("error : a mutex creation failed\n", 2);
		pthread_mutex_destroy(&data->mutex_for_print);
		pthread_mutex_destroy(&data->mutex_for_data_access);
		pthread_mutex_destroy(&data->mutex_for_stop);
		exit(EXIT_FAILURE);
	}
	if (pthread_mutex_init(&data->mutex_for_satieted, NULL))
	{
		ft_putstr_fd("error : a mutex creation failed\n", 2);
		pthread_mutex_destroy(&data->mutex_for_print);
		pthread_mutex_destroy(&data->mutex_for_data_access);
		pthread_mutex_destroy(&data->mutex_for_stop);
		pthread_mutex_destroy(&data->mutex_for_death);
		exit(EXIT_FAILURE);
	}
}
