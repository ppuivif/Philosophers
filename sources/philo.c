/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:35:53 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 09:09:00 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		if (pthread_create(&philo[i].philo_thread, NULL, \
		(void *)routine, &philo[i]))
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

	data_mutex_init_1(&data);
	data_mutex_init_2(&data);
	
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
