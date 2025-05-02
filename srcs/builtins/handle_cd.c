/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:26 by alex              #+#    #+#             */
/*   Updated: 2025/05/02 12:23:05 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	remember_path(char ***myenvp)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		mysetenv("OLDPWD", cwd, myenvp);
		free(cwd);
	}
}

char	*choose_path_minus(char **myenvp)
{
	char	*value;
	char	buffer[128];

	value = my_getenv(myenvp, "OLDPWD");
	if (value)
	{
		write(1, value, ft_strlen(value));
		write(1, "\n", 1);
		return (ft_strdup(value));
	}
	if (getcwd(buffer, sizeof(buffer)) == NULL)
	{
		perror("minishell: getcwd");
		exit(EXIT_FAILURE);
	}
	return (ft_strdup(buffer));
}

char	*choose_path(char **arr, int i, char **myenvp)
{
	char	*path;
	char	*clean_path;

	path = NULL;
	if (i == 2)
	{
		if (ft_strcmp(arr[1], "~") == 0)
			i = 1;
		else if (ft_strcmp(arr[1], "-") == 0)
			i = -1;
	}
	if (i == -1)
		return (choose_path_minus(myenvp));
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
	clean_path = remove_quotes(arr[1]);
	return (clean_path);
}

int	get_arr(char ***arr, int *i, char *line)
{
	*arr = ft_split(line, ' ');
	if (!*arr)
		return (free(line), exit(1), 1);
	*i = 0;
	while ((*arr)[*i])
		(*i)++;
	if (*i > 2)
	{
		printf("minishell: cd: too many arguments\n");
		return (1);
	}
	return (0);
}

int	handle_cd(char *line, char ***myenvp)
{
	char	**arr;
	char	*path;
	char	*new_pwd;
	int		i;

	if (get_arr(&arr, &i, line))
		return (free_arr(arr), 1);
	path = choose_path(arr, i, *myenvp);
	if (!path)
		return (free_arr(arr), 1);
	remember_path(myenvp);
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (free(path), free_arr(arr), 1);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		mysetenv("PWD", new_pwd, myenvp);
		free(new_pwd);
	}
	free(path);
	free_arr(arr);
	return (0);
}
