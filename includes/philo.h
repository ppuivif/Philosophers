/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:30:45 by ppuivif           #+#    #+#             */
/*   Updated: 2024/09/19 15:17:59 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H 
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_data	t_data;
typedef struct s_data
{
	int				philo_nmemb;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meals_number_for_satieted;
	long int		start_time;
	int				number_of_satieted_philos;
	int				one_philo_is_dead;
	bool			stop_routine;
	pthread_mutex_t	mutex_for_data_access;
	pthread_mutex_t	mutex_for_print;
	pthread_mutex_t	mutex_for_stop;
	pthread_mutex_t	mutex_for_death;
	pthread_mutex_t	mutex_for_satieted;
}	t_data;

typedef struct s_fork	t_fork;
typedef struct s_fork
{
	int				fork_id;
	bool			fork_is_available;
	pthread_mutex_t	mutex_for_fork;
}	t_fork;

typedef struct s_philo	t_philo;
typedef struct s_philo
{
	int				philo_id;
	pthread_t		philo_thread;
	long int		beginning_of_last_meal;
	int				meals_count;
	t_fork			*right_fork;	
	t_fork			*left_fork;
	t_data			*data;
	t_fork			*fork;
}	t_philo;

/*
* utils.c
*/

void		ft_putstr_fd(char *s, int fd);
bool		ft_isspace(int c);
void		*ft_calloc(size_t nmemb, size_t size);
int			ft_strcmp(char *s1, char *s2);

/*
* get_time.c
*/

void		get_time_before_death_and_run(t_philo *philo, long int duration);
long int	get_current_time_ms(t_data *data, t_fork *fork, t_philo *philo);
long int	get_timestamp_ms(t_philo *philo, long int start_time);
void		ft_usleep_ms(t_philo *philo, long int duration);

/*
* parsing.c
*/

void		check_arguments_and_fill_data_struct(int argc, char **argv, \
			t_data *data);

/*
* mutex_init.c
*/

void		data_mutex_init_1(t_data *data);
void		data_mutex_init_2(t_data *data);

/*
* structs_init.c
*/

void		forks_struct_init(t_data *data, t_fork **fork);
int			philos_struct_init(t_data *data, t_fork *fork, t_philo **philo);

/*
* print.c
*/

long int	print_message(t_philo *philo, int philo_id, long int start_time, \
char *message);

/*
* error_handling_1.c
*/

void		error_allocation_forks_struct_and_exit(t_data *data);
void		error_fork_mutex_creation_and_exit(t_data *data, t_fork *fork, \
			int nmemb);
void		error_allocation_philo_struct_and_exit(t_data *data, \
			t_fork *fork, int nmemb);
void		error_philo_thread_creation(t_data *data, t_fork *fork, \
			t_philo *philo, int nmemb);

/*
* error_handling_2.c
*/

void		error_philo_thread_join(t_data *data, t_fork *fork, \
			t_philo *philo);
void		error_philo_thread_detach(t_data *data, t_fork *fork, \
			t_philo *philo);

/*
* structs_free.c
*/

void		mutex_destroy_and_free_all(t_data *data, t_fork *fork, \
			t_philo *philo);

/*
* mutex_destroy.c
*/

void		all_fork_mutex_destroy(t_fork *fork, int nmemb);
void		all_mutex_destroy(t_data *data, t_fork *fork, int nmemb);

/*
* routine.c
*/

void		*routine(t_philo *philo);

/*
* take_forks.c
*/

void		take_forks(t_philo *philo, int philo_id, long int start_time);

/*
* supervisor.c
*/

void		supervisor(t_philo *philo, long int start_time);

/*
* Tests
*/

void		data_struct_print(t_data data);
void		forks_struct_print(t_fork *fork, t_data data);
void		philos_struct_print(t_philo *philo, t_data data);
void		print_fork_statement(t_fork *fork, t_data data);

#endif