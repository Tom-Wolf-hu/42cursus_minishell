/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:17 by alex              #+#    #+#             */
/*   Updated: 2025/04/13 17:06:47 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_getcwd(char *line, int fd)
{
	char	buffer[128];
	size_t	size;

	size = 128;
	if (getcwd(buffer, size) == NULL)
	{
		perror("minishell: getcwd");
		free(line);
		exit(EXIT_FAILURE);
	// return (free(line), exit(EXIT_FAILURE), 1);
	}
	write(fd, buffer, ft_strlen(buffer));
	write(fd, "\n", 1);
	return (0);
}
