/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:56:59 by omalovic          #+#    #+#             */
/*   Updated: 2025/05/05 09:44:37 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	finish_write_cmd_path(char **buffer, char *path, char *cmd)
{
	int	len;

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
	char	*path;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
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
