/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:36:51 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/29 13:38:15 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_status(int *status)
{
	if (g_status == 130)
	{
		*status = g_status;
		g_status = 0;
	}
}

char	**copy_arr(char **arr)
{
	int		count;
	char	**result;
	int		i;

	count = 0;
	while (arr[count])
		count++;
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (exit(1), NULL);
	i = 0;
	while (i < count)
	{
		result[i] = ft_strdup(arr[i]);
		if (!result[i])
			return (free_arr(result), exit(1), NULL);
		i++;
	}
	result[i] = NULL;
	return (result);
}

void	handle_gnl(char **line, int *status)
{
	*line = get_next_line(STDIN_FILENO);
	if (!*line)
		return (perror("malloc"), *status = 1, (void)0);
	*line = ft_strtrim(*line, "\n");
	if (!*line)
		return (perror("malloc"), *status = 1, (void)0);
}
