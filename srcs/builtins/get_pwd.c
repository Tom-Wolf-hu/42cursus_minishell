/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:17 by alex              #+#    #+#             */
/*   Updated: 2025/03/17 19:46:19 by alex             ###   ########.fr       */
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
		return (free(line), exit(EXIT_FAILURE), 1);
	}
	// printf("%s\n", buffer);
	write(fd, buffer, ft_strlen(buffer));
	write(fd, "\n", 1);
	return (0);
}
