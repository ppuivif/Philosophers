/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_destroy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:18:13 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/18 15:27:26 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	all_fork_mutex_destroy(t_fork *fork, int nmemb)
{
	int	i;

	i = 0;
	while (i < nmemb)
	{
		pthread_mutex_destroy(&(&fork[i])->mutex_for_fork);
		i++;
	}
}

void    all_mutex_destroy(t_data *data, t_fork *fork, int nmemb)
{
	pthread_mutex_destroy(&data->mutex_for_print);
	pthread_mutex_destroy(&data->mutex_for_data_access);
	pthread_mutex_destroy(&data->mutex_for_stop);
	pthread_mutex_destroy(&data->mutex_for_death);
	pthread_mutex_destroy(&data->mutex_for_satieted);
	all_fork_mutex_destroy(fork, nmemb);
}
