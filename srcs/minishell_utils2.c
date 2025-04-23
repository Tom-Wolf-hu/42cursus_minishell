/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:56:59 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/20 17:38:48 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	finish_write_cmd_path(char **buffer, char *path, char *cmd)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	if (!path || !cmd)
		return (0);
	len = ft_strlen(path) + ft_strlen(cmd);
	*buffer = malloc(len + 2);
	if (!*buffer)
		return (0);
	ft_strcpy(*buffer, path);
	ft_strcat(*buffer, "/");
	ft_strcat(*buffer, cmd);
	return (1);
}

char	*find_cmd_in_paths(char **path_arr, char *cmd)
{
	char	*buffer;
	int		i;

	i = 0;
	while (path_arr[i])
	{
		if (!finish_write_cmd_path(&buffer, path_arr[i], cmd))
			return (free_arr(path_arr), NULL);
		if (access(buffer, X_OK) == 0)
		{
			free_arr(path_arr);
			return (buffer);
		}
		free(buffer);
		i++;
	}
	return (free_arr(path_arr), NULL);
}

char	*get_command_path(char *cmd, char **myenvp)
{
	char	**path_arr;
	char	*buffer;
	char	*path;
	int		i;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	buffer = NULL;
	path = find_var_value("PATH", myenvp);
	if (!path)
		return (NULL);
	path_arr = ft_split(path, ':');
	if (!path_arr || !*path_arr)
		return (free(path), NULL);
	return (free(path), find_cmd_in_paths(path_arr, cmd));
}

char	**get_commands(char *cmd, char *temp)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	if (cmd[i] == '|')
	{
		temp = malloc(ft_strlen(cmd) + 2);
		if (!temp)
			return (exit(1), NULL);
		temp[i] = ' ';
		i++;
		while (cmd[j])
		{
			temp[i] = cmd[j];
			j++;
			i++;
		}
		temp[i] = '\0';
		return (ft_split(temp, '|'));
	}
	return (ft_split(cmd, '|'));
}
