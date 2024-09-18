/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:35:53 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/18 19:54:41 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	get_time_before_death_and_run(t_philo *philo, long int duration)
{
	long int	completed_duration_until_last_meal;
	long int	remaining_duration_before_death;

	completed_duration_until_last_meal = get_timestamp_ms(philo, \
	philo->data->start_time) - philo->beginning_of_last_meal;
	remaining_duration_before_death = philo->data->time_to_die - completed_duration_until_last_meal;
	if (remaining_duration_before_death <= 0 || remaining_duration_before_death < duration)
	{
		duration = remaining_duration_before_death;
		ft_usleep_ms(philo, duration);
		pthread_mutex_lock(&(philo)->data->mutex_for_death);
		philo->data->one_philo_is_dead = philo->philo_id;
		pthread_mutex_unlock(&(philo)->data->mutex_for_death);
	}
	else
		ft_usleep_ms(philo, duration);
}

static void	take_meal_for_odd(t_philo *philo, int philo_id, long int start_time)
{
	pthread_mutex_lock(&philo->right_fork->mutex_for_fork);
	print_message(philo, philo_id, start_time, "has taken a fork");
	get_time_before_death_and_run(philo, 0);
	pthread_mutex_lock(&philo->left_fork->mutex_for_fork);
	print_message(philo, philo_id, start_time, "has taken a fork");
	get_time_before_death_and_run(philo, 0);
	philo->beginning_of_last_meal = print_message(philo, philo_id, \
	start_time, "is eating");
	get_time_before_death_and_run(philo, philo->data->time_to_eat);
	pthread_mutex_lock(&(philo)->data->mutex_for_satieted);
	philo->meals_count++;
	if (philo->meals_count == philo->data->meals_number_for_satieted)
		philo->data->number_of_satieted_philos += 1;
	pthread_mutex_unlock(&(philo)->data->mutex_for_satieted);
	pthread_mutex_unlock(&philo->left_fork->mutex_for_fork);
	pthread_mutex_unlock(&philo->right_fork->mutex_for_fork);
}

static void	take_meal_for_even(t_philo *philo, int philo_id, long int start_time)
{
	pthread_mutex_lock(&philo->left_fork->mutex_for_fork);
	print_message(philo, philo_id, start_time, "has taken a fork");
	get_time_before_death_and_run(philo, 0);
	pthread_mutex_lock(&philo->right_fork->mutex_for_fork);
	print_message(philo, philo_id, start_time, "has taken a fork");
	get_time_before_death_and_run(philo, 0);
	philo->beginning_of_last_meal = print_message(philo, philo_id, \
	start_time, "is eating");
	get_time_before_death_and_run(philo, philo->data->time_to_eat);
	pthread_mutex_lock(&(philo)->data->mutex_for_satieted);
	philo->meals_count++;
	if (philo->meals_count == philo->data->meals_number_for_satieted)
		philo->data->number_of_satieted_philos += 1;
	pthread_mutex_unlock(&(philo)->data->mutex_for_satieted);
	pthread_mutex_unlock(&philo->right_fork->mutex_for_fork);
	pthread_mutex_unlock(&philo->left_fork->mutex_for_fork);
}

static void	take_a_rest(t_philo *philo, int philo_id, long int start_time)
{
	print_message(philo, philo_id, start_time, "is sleeping");
	get_time_before_death_and_run(philo, philo->data->time_to_sleep);
}

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
	int meals_number_for_satieted;
	int	index_philo_dead;
	
	pthread_mutex_lock(&philo->data->mutex_for_data_access);
	meals_number_for_satieted = philo->data->meals_number_for_satieted;
	pthread_mutex_unlock(&philo->data->mutex_for_data_access);
	index_philo_dead = 0;
	while (1)
	{
		if ((meals_number_for_satieted && check_if_philos_satieted(philo) == true) || \
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

void run_routine(t_philo *philo, int philo_id, long int start_time, bool *stop_routine)
{
		pthread_mutex_lock(&philo->data->mutex_for_stop);
		*stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (*stop_routine == false && philo->data->philo_nmemb % 2)
			take_meal_for_even(philo, philo_id, start_time);
		else if (*stop_routine == false && !(philo_id % 2))
			take_meal_for_odd(philo, philo_id, start_time);
		else if (*stop_routine == false && (philo_id % 2))
			take_meal_for_even(philo, philo_id, start_time);
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
		pthread_mutex_lock(&philo->data->mutex_for_stop);
		*stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
}

static void	*routine(t_philo *philo)
{
	int			philo_id;
	long int	start_time;
	bool		stop_routine;

	philo_id = philo->philo_id;
	start_time = philo->data->start_time;
	stop_routine = false;
	if (philo_id % 2 == 0)
	{
		print_message(philo, philo_id, start_time, "is thinking");
		usleep(10000);
	}
	if (philo->data->philo_nmemb == 1)
	{
		print_message(philo, philo_id, start_time, "has taken a fork");
		ft_usleep_ms(philo, philo->data->time_to_die);
		get_time_before_death_and_run(philo, 0);
		usleep(500);
	}
	while (stop_routine == false)
		run_routine(philo, philo_id, start_time, &stop_routine);
	return (NULL);
}

static void	run_diner(t_data *data, t_fork *fork, t_philo *philo)
{
	int			i;
	int			philo_nmemb;
	long int	start_time;

	i = 0;
	philo_nmemb = data->philo_nmemb;
	start_time = data->start_time;
	
	while (i < philo_nmemb)
	{
		if (pthread_create(&philo[i].philo_thread, NULL, (void *)routine, &philo[i]))
			error_philo_thread_creation(data, fork, philo, i);
		i++;
	}
	supervisor(philo, start_time);
	i = 0;
	while (i < philo_nmemb)
	{
		if (pthread_join(philo[i].philo_thread, NULL))
			error_philo_thread_join(data, fork, philo);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philo;
	t_fork	*fork;

	philo = NULL;
	fork = NULL;

	check_arguments_and_fill_data_struct(argc, argv, &data);

//	data_struct_print (data);

	data_mutex_init(&data);
	
	forks_struct_init(&data, &fork);

//	forks_struct_print (fork, data);

	data.start_time = get_current_time_ms(&data, fork, NULL);
	data.one_philo_is_dead = 0;
	data.stop_routine = false;
	data.number_of_satieted_philos = 0;
	
	philos_struct_init(&data, fork, &philo);

//	data_struct_print (*philo->data);

//	philos_struct_print (philo, data);

	run_diner(&data, fork, philo);

	mutex_destroy_and_free_all(philo->data, philo->fork, philo);

	return (0);
}
