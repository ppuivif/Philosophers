/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 10:09:20 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 15:15:40 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_philo_thread_join(t_data *data, t_fork *fork, t_philo *philo)
{
	ft_putstr_fd("error : a thread join failed\n", 2);
	mutex_destroy_and_free_all(data, fork, philo);
	exit(EXIT_FAILURE);
}

void	error_philo_thread_detach(t_data *data, t_fork *fork, t_philo *philo)
{
	ft_putstr_fd("error : a thread detach failed\n", 2);
	mutex_destroy_and_free_all(data, fork, philo);
	exit(EXIT_FAILURE);
}
