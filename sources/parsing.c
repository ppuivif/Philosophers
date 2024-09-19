/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 09:09:41 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 09:11:46 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_data(int index, int valid_value, t_data *data)
{
	data->meals_number_for_satieted = 0;
	if (index == 1)
		data->philo_nmemb = valid_value;
	else if (index == 2)
		data->time_to_die = valid_value;
	else if (index == 3)
		data->time_to_eat = valid_value;
	else if (index == 4)
		data->time_to_sleep = valid_value;
	else if (index == 5)
		data->meals_number_for_satieted = valid_value;
}

static int	ft_atoi_long_positive(char *str)
{
	int			i;
	long int	nb;

	i = 0;
	nb = 0;
	while (ft_isspace(str[i]) == true)
		i++;
	if (str[i] == 45)
		return (-1);
	if (str[i] == 43)
		i++;
	if (str[i] == 0)
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	if (nb > INT_MAX)
		return (-1);
	return (nb);
}

static void	check_and_convert_arguments(char **argv, t_data *data)
{
	int	i;
	int	tmp;

	i = 1;
	while (argv[i])
	{
		tmp = ft_atoi_long_positive(argv[i]);
		if (tmp > 0)
			init_data(i, tmp, data);
		else
		{
			ft_putstr_fd("Error : the value of at least one argument \
is not correct\n", 2);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	check_arguments_and_fill_data_struct(int argc, char **argv, \
			t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("Error : number of arguments provided \
is not correct\n", 2);
		exit (EXIT_FAILURE);
	}
	check_and_convert_arguments(argv, data);
}
