/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:26 by alex              #+#    #+#             */
/*   Updated: 2025/04/15 14:33:37 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void remember_path(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		mysetenv("OLDPATH", cwd);
		free(cwd);
	}
}

char *choose_path_minus(void)
{
	char *value;
	char buffer[128];

	value = getenv("OLDPATH");
	if (value)
		return (ft_strdup(value));
	if (getcwd(buffer, sizeof(buffer)) == NULL)
	{
		perror("minishell: getcwd");
		exit(EXIT_FAILURE);
	}
	return (ft_strdup(buffer));
}

char *choose_path(char **arr, int i)
{
	char *path = NULL;
	char *clean_path;

	if (i == 2)
	{
		if (ft_strcmp(arr[1], "~") == 0)
			i = 1;
		else if (ft_strcmp(arr[1], "-") == 0)
			i = -1;
	}
	if (i == -1)
		return choose_path_minus();
	if (i == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (NULL);
		}
		return (ft_strdup(path));
	}
	else
	{
		clean_path = remove_quotes(arr[1]);
		if (!clean_path)
			return (NULL);
		return (clean_path);
	}
}

int handle_cd(char *line)
{
	char	**arr;
	char	*path;
	char	*new_pwd;
	int		i;

	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1), 1);
	i = 0;
	while (arr[i])
		i++;
	if (i > 2)
	{
		printf("minishell: cd: too many arguments\n");
		free_arr(arr);
		return (1);
	}
	path = choose_path(arr, i);
	if (!path)
		return (free_arr(arr), 1);
	remember_path();
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		free(path);
		free_arr(arr);
		return 1;
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		mysetenv("PWD", new_pwd);
		free(new_pwd);
	}
	free(path);
	free_arr(arr);
	return (0);
}

// void	remember_path(char *path)
// {
// 	mysetenv("OLDPATH", path);
// }

// char *choose_path_minus(char *path)
// {
// 	char	*value;
// 	char	buffer[128];
// 	size_t	size;

// 	size = 128;
// 	value = getenv("OLDPATH");
// 	if (value)
// 		return (value);
// 	if (getcwd(buffer, size) == NULL)
// 	{
// 		perror("minishell: getcwd");
// 		return (exit(EXIT_FAILURE), 1);
// 	}
// 	value = ft_strdup(buffer);
// 	return (value);
// }

// char	*choose_path(char **arr, int i)
// {
// 	char	*path;
// 	char	*clean_path;

// 	path = NULL;
// 	if (i == 2)
// 	{
// 		if (ft_strcmp(arr[1], "~") == 0)
// 			i = 1;
// 		else if (ft_strcmp(arr[1], "-") == 0)
// 			i = -1;
// 	}
// 	if (i == -1)
// 		return (choose_path_minus(path));
// 	if (i == 1)
// 	{
// 		path = getenv("HOME");
// 		if (!path)
// 		{
// 			printf("minishell: cd: HOME not set\n");
// 			return (NULL);
// 		}
// 	}
// 	else
// 	{
// 		clean_path = remove_quotes(arr[1]);
// 		if (!clean_path)
// 			return (NULL);
// 		path = clean_path;
// 	}
// 	return (path);
// }

// int	handle_cd(char *line)
// {
// 	char	**arr;
// 	char	*path;
// 	int 	i;

// 	i = 0;
// 	arr = ft_split(line, ' ');
// 	if (!arr)
// 		return (free(line), exit(1), 1);
// 	while (arr[i])
// 		i++;
// 	if (i > 2)
// 	{
// 		printf("minishell: cd: too many arguments\n");
// 		free_arr(arr);
// 		return (1);
// 	}
// 	path = choose_path(arr, i);
// 	remember_path(path);
// 	if (!path)
// 		return (free_arr(arr), 1);
// 	if (chdir(path) != 0)
// 		return (perror("minishell: cd"), 1);
// 	free_arr(arr);
// 	if (path)
// 		free(path);
// 	return (0);
// }
