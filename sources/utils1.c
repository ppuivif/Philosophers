/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppuivif <ppuivif@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 07:47:02 by ppuivif           #+#    #+#             */
/*   Updated: 2024/07/23 12:02:42 by ppuivif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (s != 0)
	{
		while (s[i])
		{
			write(fd, &s[i], 1);
			i++;
		}
	}
}

bool	ft_isspace(int c)
{
	if (((c >= 9) && (c <= 13)) || c == 32)
		return (true);
	return (false);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = nmemb * size;
	if (!size || !nmemb)
		return (malloc(1));
	if (j > 4294967295 || (size > 65535 && nmemb > 65535))
		return (NULL);
	str = (char *)malloc(j);
	if (str)
	{
		while (i < j)
		{
			str[i] = '\0';
			i++;
		}
		return ((void *) str);
	}
	return (NULL);
}
