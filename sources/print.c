/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 07:37:40 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/18 18:49:14 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <string.h>


long int	print_message(t_philo *philo, int philo_id, long int start_time, \
char *message)
{
	long int	timestamp;
	bool		stop_routine;

	stop_routine = false;
	pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
	pthread_mutex_unlock(&philo->data->mutex_for_stop);
	
	if (stop_routine == true && strcmp(message, "is dead") != 0)
		return (0);//to modify
	timestamp = get_timestamp_ms(philo, start_time);
	pthread_mutex_lock(&philo->data->mutex_for_print);
	printf("%ld %d %s\n", timestamp, philo_id, message);
//	printf("%d ", philo_id);
//	printf("%s\n", message);
	pthread_mutex_unlock(&philo->data->mutex_for_print);
	return (timestamp);
}