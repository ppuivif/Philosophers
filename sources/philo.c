/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:35:53 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/18 17:19:04 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	get_time_before_death_and_run(t_philo *philo, long int duration, int nb)
{
	long int	completed_duration_until_last_meal;
	long int	remaining_duration_before_death;

	(void)nb;

	completed_duration_until_last_meal = get_timestamp_ms(philo, philo->data->start_time) - \
	philo->beginning_of_last_meal;
//	printf("philo %d - completed_duration_until_last_meal : %ld\n", philo->philo_id, completed_duration_until_last_meal);
	
	remaining_duration_before_death = philo->data->time_to_die - completed_duration_until_last_meal;
//	printf("philo %d - remaining_duration_before_death : %ld\n", philo->philo_id, remaining_duration_before_death);

//	printf("philo %d - duration : %ld\n", philo->philo_id, duration);
	
	if (remaining_duration_before_death <= 0)
	{
//		pthread_mutex_lock(&(philo)->data->mutex_for_print);
//		printf("nb : %d\n", nb);
//		pthread_mutex_unlock(&(philo)->data->mutex_for_print);
		pthread_mutex_lock(&(philo)->data->mutex_for_death);
		philo->data->one_philo_is_dead = philo->philo_id;
		pthread_mutex_unlock(&(philo)->data->mutex_for_death);
	}

//	if (remaining_duration_before_death <= 0 || remaining_duration_before_death < duration)
	else if (remaining_duration_before_death < duration)
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

static void	take_forks(t_philo *philo, int philo_id, long int start_time)
{
	int	number_of_available_forks;
	bool	stop_routine;

	number_of_available_forks = 0;
	stop_routine = true;
//	while (number_of_available_forks < 2 && philo->stop_routine == false) // si pas cette derniere verif, reste bloqué dans la boucle qd nbre repas atteints
	while (number_of_available_forks < 2) // si pas cette derniere verif, reste bloqué dans la boucle qd nbre repas atteints
	{
		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (stop_routine == true)
			break;


		if (number_of_available_forks == 0)
		{
			get_time_before_death_and_run(philo, 0, 1);
			pthread_mutex_lock(&philo->right_fork->mutex_for_fork);
			if (philo->right_fork->fork_is_available == true)
			{
				philo->right_fork->fork_is_available = false;
				number_of_available_forks++;
				print_message(philo, philo_id, start_time, "has taken a fork");
			}
			pthread_mutex_unlock(&philo->right_fork->mutex_for_fork);
		}

		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (stop_routine == true)
			break;

//		usleep(500);
		if (number_of_available_forks == 1)
		{
			get_time_before_death_and_run(philo, 0, 2);
			if (philo->left_fork)
			{
				pthread_mutex_lock(&philo->left_fork->mutex_for_fork);
				if (philo->left_fork->fork_is_available == true)
				{
					philo->left_fork->fork_is_available = false;
					number_of_available_forks++;
					print_message(philo, philo_id, start_time, "has taken a fork");
				}
				pthread_mutex_unlock(&philo->left_fork->mutex_for_fork);
			}
		}
		usleep(100);
	}
}

static void	take_meal(t_philo *philo, int philo_id, long int start_time)
{
	philo->beginning_of_last_meal = print_message(philo, philo_id, \
	start_time, "is eating");
	
	
	get_time_before_death_and_run(philo, philo->data->time_to_eat, 3);

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
	get_time_before_death_and_run(philo, philo->data->time_to_sleep, 4);
//	usleep(500);
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

static bool	check_if_one_philo_dead(t_philo *philo, long int start_time)
{
	int	index_philo_dead;
	
	index_philo_dead = 0;
	pthread_mutex_lock(&philo->data->mutex_for_death);
	index_philo_dead = philo->data->one_philo_is_dead;
	pthread_mutex_unlock(&philo->data->mutex_for_death);
	if (index_philo_dead > 0)
	{
		print_message(philo, index_philo_dead, start_time, "is dead");
		return (true);
	}
	return (false);
}

void	supervisor(t_philo *philo, int philo_nmemb, long int start_time)
{
	int meals_number_for_satieted;
	
//	(void)start_time;
	(void)philo_nmemb;
	
	pthread_mutex_lock(&philo->data->mutex_for_data_access);
	meals_number_for_satieted = philo->data->meals_number_for_satieted;
	pthread_mutex_unlock(&philo->data->mutex_for_data_access);
	while (1)
	{
/*		if ((meals_number_for_satieted && check_if_philos_satieted(philo, \
		philo_nmemb, meals_number_for_satieted, &index_philo_satieted) == true))*/

		if ((meals_number_for_satieted && check_if_philos_satieted(philo) == true) || \
		check_if_one_philo_dead(philo, start_time) == true)
		{
			pthread_mutex_lock(&philo->data->mutex_for_stop);
			philo->data->stop_routine = true;
			pthread_mutex_unlock(&philo->data->mutex_for_stop);
			return ;
		}
		usleep(500);
	}
}

/*bool	check_stop_routine(t_philo *philo)
{
	bool	stop_routine;
	
	stop_routine = false;
	
	pthread_mutex_lock(&philo->data->mutex_for_stop);
	if (philo->data->stop_routine == true)
		stop_routine = true;
	pthread_mutex_unlock(&philo->data->mutex_for_stop);
	
	return (stop_routine);
}*/

static void	*routine(t_philo *philo)
{
	int			philo_id;
	long int	start_time;
	bool		stop_routine;

//	pthread_mutex_lock(&philo->mutex_for_philo);
	philo_id = philo->philo_id;
//	pthread_mutex_unlock(&philo->mutex_for_philo);

	pthread_mutex_lock(&philo->data->mutex_for_data_access);
	start_time = philo->data->start_time;
	pthread_mutex_unlock(&philo->data->mutex_for_data_access);

	stop_routine = false;


	if (philo_id % 2 == 0)
	{
		print_message(philo, philo_id, start_time, "is thinking");
		usleep(500);
	}

//		ft_usleep_ms(philo, philo->data->time_to_eat);

	while (stop_routine == false)
	{
		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (stop_routine == false)
			take_forks(philo, philo_id, start_time);
//		else
//			break;


		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (stop_routine == false)
			take_meal(philo, philo_id, start_time);
//		else
//			break;

		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (stop_routine == false)
			take_a_rest(philo, philo_id, start_time);
//		else
//			break;


		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
		if (stop_routine == false)
			print_message(philo, philo_id, start_time, "is thinking");
//		else
//			break;
		
		pthread_mutex_lock(&philo->data->mutex_for_stop);
		stop_routine = philo->data->stop_routine;
		pthread_mutex_unlock(&philo->data->mutex_for_stop);
	}
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
	
	supervisor(philo, philo_nmemb, start_time);
	
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
