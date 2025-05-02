/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:44:52 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/02 14:45:31 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_gnl(char **line, int *status)
{
	*line = get_next_line(STDIN_FILENO);
	if (!*line)
		return (perror("malloc"), *status = 1, (void)0);
	*line = ft_strtrim(*line, "\n");
	if (!*line)
		return (perror("malloc"), *status = 1, (void)0);
}

int	if_exit(char *line)
{
	if (ft_strcmp(line, "exit") == 0
		|| ft_strncmp(line, "exit ", 5) == 0
		|| ft_strcmp(line, "\"exit\"") == 0
		|| ft_strncmp(line, "\"exit\" ", 7) == 0
		|| ft_strcmp(line, "'exit'") == 0
		|| ft_strncmp(line, "'exit' ", 7) == 0)
		return (1);
	return (0);
}
