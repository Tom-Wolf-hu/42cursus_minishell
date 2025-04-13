/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:44 by alex              #+#    #+#             */
/*   Updated: 2025/04/13 17:53:07 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	print_env(int fd)
{
	int			i;
	extern char	**environ;

	i = 0;
	while (environ[i])
	{
		write(fd, environ[i], ft_strlen(environ[i]));
		write(fd, "\n", 1);
		i++;
	}
	return (0);
}
