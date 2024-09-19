/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:35:53 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 09:15:08 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_if_philos_satieted(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->mutex_for_satieted);
	if (philo->data->number_of_satieted_philos == philo->data->philo_nmemb)
	{
		pthread_mutex_unlock(&philo->data->mutex_for_satieted);
		return (true);
	}
	pthread_mutex_unlock(&philo->data->mutex_for_satieted);
	return (false);
}

static bool	check_if_one_philo_dead(t_philo *philo, int	*index_philo_dead)
{
	pthread_mutex_lock(&philo->data->mutex_for_death);
	*index_philo_dead = philo->data->one_philo_is_dead;
	pthread_mutex_unlock(&philo->data->mutex_for_death);
	if (*index_philo_dead > 0)
		return (true);
	return (false);
}

void	supervisor(t_philo *philo, long int start_time)
{
	int	meals_number_for_satieted;
	int	index_philo_dead;

	pthread_mutex_lock(&philo->data->mutex_for_data_access);
	meals_number_for_satieted = philo->data->meals_number_for_satieted;
	pthread_mutex_unlock(&philo->data->mutex_for_data_access);
	index_philo_dead = 0;
	while (1)
	{
		if ((meals_number_for_satieted && \
		check_if_philos_satieted(philo) == true) || \
		check_if_one_philo_dead(philo, &index_philo_dead) == true)
		{
			pthread_mutex_lock(&philo->data->mutex_for_stop);
			philo->data->stop_routine = true;
			pthread_mutex_unlock(&philo->data->mutex_for_stop);
			if (index_philo_dead > 0)
				print_message(philo, index_philo_dead, start_time, "is dead");
			return ;
		}
		usleep(500);
	}
}
