#include "philo.h"

void	data_struct_print(t_data data)
{
	printf("number of philo : %d\n", data.philo_nmemb);
	printf("time to die : %d\n", data.time_to_die);
	printf("time to eat : %d\n", data.time_to_eat);
	printf("time to sleep : %d\n", data.time_to_sleep);
	printf("meals number : %d\n", data.meals_number_for_satieted);
	printf("\n");
}

void	forks_struct_print(t_fork *fork, t_data data)
{
	int	i;

	i = 0;
	while (i < data.philo_nmemb)
	{
		printf("index fork[%d] : %d\n", i, fork[i].fork_id);
		i++;
	}
}

void	philos_struct_print(t_philo *philo, t_data data)
{
	int	i;

	i = 0;
	while (i < data.philo_nmemb)
	{
		printf("index philo[%d] : %d\n", i, philo[i].philo_id);
		printf("philo[%d] - index right_fork : %d\n", i, philo[i].right_fork->fork_id);
		printf("fork%d is available : %s\n", i + 1, philo->fork->fork_is_available ? "true" : "false");
		printf("philo[%d] - index left_fork : %d\n", i, philo[i].left_fork->fork_id);
		printf("fork%d is available : %s\n", i + 1, philo->fork->fork_is_available ? "true" : "false");
		printf("\n");
		i++;
	}
}

void	print_fork_statement(t_fork *fork, t_data data)
{
	int	i;

	i = 0;
	while (i < data.philo_nmemb)
	{
		printf("fork%d is available : %s\n", i + 1, (&fork[i])->fork_is_available ? "true" : "false");
		i++;
	}
	printf("\n");
}

