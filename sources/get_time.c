/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_time.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:34:26 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/16 19:55:27 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	get_current_time_ms(t_data *data, t_fork *fork, t_philo *philo)
{
	struct timeval	time;
	long int		current_time;

	if (gettimeofday(&time, NULL) == -1)
	{
		ft_putstr_fd("error : gettimeofday failed\n", 2);
		mutex_destroy_and_free_all(data, fork, philo);//philo (or *philo) to verify
		exit(EXIT_FAILURE);
	}
	current_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (current_time);
}

long int	get_timestamp_ms(t_philo *philo, long int start_time)
{
	long int	current_time;
	long int	timestamp;

	current_time = get_current_time_ms(philo->data, philo->fork, philo);
	timestamp = (current_time - start_time);
	return (timestamp);
}

void	ft_usleep_ms(t_philo *philo, long int duration)
{
	long int	start_time;
	long int	stop;

	if (duration == 0)
		return ;
	start_time = get_current_time_ms(philo->data, philo->fork, philo);
	

	stop = duration + start_time;
	while(duration > 5000)
	{
		duration /= 2;
		usleep(duration*1000);
		duration = stop - get_current_time_ms(philo->data, philo->fork, philo);
	}
	while (get_current_time_ms(philo->data, philo->fork, philo) < stop)
		usleep(500);
}
