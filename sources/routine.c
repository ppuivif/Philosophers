/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:35:53 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 15:13:44 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_meal(t_philo *philo, int philo_id, long int start_time)
{
	philo->beginning_of_last_meal = print_message(philo, philo_id, \
	start_time, "is eating");
	get_time_before_death_and_run(philo, philo->data->time_to_eat);
	pthread_mutex_lock(&(philo)->data->mutex_for_satieted);
	philo->meals_count++;
	if (philo->meals_count == philo->data->meals_number_for_satieted)
		philo->data->number_of_satieted_philos += 1;
	pthread_mutex_unlock(&(philo)->data->mutex_for_satieted);
	pthread_mutex_lock(&philo->right_fork->mutex_for_fork);
	philo->right_fork->fork_is_available = true;
	pthread_mutex_unlock(&philo->right_fork->mutex_for_fork);
	pthread_mutex_lock(&philo->left_fork->mutex_for_fork);
	philo->left_fork->fork_is_available = true;
	pthread_mutex_unlock(&philo->left_fork->mutex_for_fork);
}

static void	take_a_rest(t_philo *philo, int philo_id, long int start_time)
{
	print_message(philo, philo_id, start_time, "is sleeping");
	get_time_before_death_and_run(philo, philo->data->time_to_sleep);
}

static void	run_routine(t_philo *philo, int philo_id, long int start_time, \
bool *stop_routine)
{
	pthread_mutex_lock(&philo->data->mutex_for_stop);
	*stop_routine = philo->data->stop_routine;
	pthread_mutex_unlock(&philo->data->mutex_for_stop);
	if (*stop_routine == false)
		take_forks(philo, philo_id, start_time);
	pthread_mutex_lock(&philo->data->mutex_for_stop);
	*stop_routine = philo->data->stop_routine;
	pthread_mutex_unlock(&philo->data->mutex_for_stop);
	if (*stop_routine == false)
		take_meal(philo, philo_id, start_time);
	pthread_mutex_lock(&philo->data->mutex_for_stop);
	*stop_routine = philo->data->stop_routine;
	pthread_mutex_unlock(&philo->data->mutex_for_stop);
	if (*stop_routine == false)
		take_a_rest(philo, philo_id, start_time);
	pthread_mutex_lock(&philo->data->mutex_for_stop);
	*stop_routine = philo->data->stop_routine;
	pthread_mutex_unlock(&philo->data->mutex_for_stop);
	if (*stop_routine == false)
		print_message(philo, philo_id, start_time, "is thinking");
}

void	*routine(t_philo *philo)
{
	int			philo_id;
	long int	start_time;
	bool		stop_routine;

	philo_id = philo->philo_id;
	start_time = philo->data->start_time;
	stop_routine = false;
	print_message(philo, philo_id, start_time, "is thinking");
	if (philo_id % 2 == 0)
		usleep(10000);
	while (stop_routine == false)
		run_routine(philo, philo_id, start_time, &stop_routine);
	return (NULL);
}
