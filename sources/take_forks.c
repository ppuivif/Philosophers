/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:35:53 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 15:25:03 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_right_fork(t_philo *philo, int philo_id, long int start_time, \
int *number_of_available_forks)
{
	get_time_before_death_and_run(philo, 0);
	pthread_mutex_lock(&philo->right_fork->mutex_for_fork);
	if (philo->right_fork->fork_is_available == true)
	{
		philo->right_fork->fork_is_available = false;
		(*number_of_available_forks)++;
		pthread_mutex_unlock(&philo->right_fork->mutex_for_fork);
		print_message(philo, philo_id, start_time, "has taken a fork");
	}
	else
		pthread_mutex_unlock(&philo->right_fork->mutex_for_fork);
}

static void	take_left_fork(t_philo *philo, int philo_id, long int start_time, \
int *number_of_available_forks)
{
	get_time_before_death_and_run(philo, 0);
	if (philo->left_fork)
	{
		pthread_mutex_lock(&philo->left_fork->mutex_for_fork);
		if (philo->left_fork->fork_is_available == true)
		{
			philo->left_fork->fork_is_available = false;
			(*number_of_available_forks)++;
			pthread_mutex_unlock(&philo->left_fork->mutex_for_fork);
			print_message(philo, philo_id, start_time, "has taken a fork");
		}
		else
			pthread_mutex_unlock(&philo->left_fork->mutex_for_fork);
	}
}

void	take_forks(t_philo *philo, int philo_id, long int start_time)
{
	int		number_of_available_forks;
	bool	stop_routine;

	number_of_available_forks = 0;
	stop_routine = true;
	while (number_of_available_forks < 2)
	{
		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (stop_routine == true)
			break ;
		if (number_of_available_forks == 0)
			take_right_fork(philo, philo_id, start_time, \
			&number_of_available_forks);
		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (stop_routine == true)
			break ;
		if (number_of_available_forks == 1)
			take_left_fork(philo, philo_id, start_time, \
			&number_of_available_forks);
		usleep(100);
	}
}
