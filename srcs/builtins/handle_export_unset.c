/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_unset.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:49:13 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/15 17:58:58 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern char	**environ;
char		**g_env_vars = NULL;
int			g_env_size = 0;

int	find_var_in_env(char *name)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	add_user_env_var(char *name)
{
	int		i;
	char	**new_list;

	i = 0;
	while (i < g_env_size)
	{
		if (ft_strncmp(g_env_vars[i], name, ft_strlen(name)) == 0
			&& g_env_vars[i][ft_strlen(name)] == '\0')
			return (0);
		i++;
	}
	new_list = ft_realloc(g_env_vars, sizeof(char *) * (g_env_size + 1));
	if (!new_list)
		return (perror("realloc g_env_vars"), 1);
	g_env_vars = new_list;
	g_env_vars[g_env_size] = ft_strdup(name);
	g_env_size++;
	return (0);
}

int	is_user_env_var(char *name)
{
	int	i;

	i = 0;
	while (i < g_env_size)
	{
		if (ft_strncmp(g_env_vars[i], name, ft_strlen(name)) == 0
			&& g_env_vars[i][ft_strlen(name)] == '\0')
			return (1);
		i++;
	}
	return (0);
}

void	free_var_after_exit(void)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		if (is_user_env_var(environ[i]))
			free(environ[i]);
		i++;
	}
	free_arr(g_env_vars);
}

int	my_unsetenv(char *name)
{
	int		index;
	char	**new_env;

	index = find_var_in_env(name);
	if (index != -1)
	{
		if (is_user_env_var(name))
			free(environ[index]);
		while (environ[index])
		{
			environ[index] = environ[index + 1];
			index++;
		}
		new_env = ft_realloc(environ, sizeof(char *) * (index));
		if (!new_env && index > 0)
			return (perror("realloc"), exit(1), 1);
		environ = new_env;
	}
	return (0);
}

// int	mysetenv(char *name, char *value)
// {
// 	int		index;
// 	char	*new_var;
// 	int		count;
// 	char	**new_env;

// 	index = find_var_in_env(name);
// 	if (value)
// 		new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
// 	else
// 		new_var = malloc(ft_strlen(name) + 1);
// 	if (!new_var)
// 		return (exit(1), 1);
// 	if (new_var == NULL)
// 		return (perror("malloc"), 1);
// 	ft_strcpy(new_var, name);
// 	if (value)
// 	{
// 		ft_strcat(new_var, "=");
// 		ft_strcat(new_var, value);
// 	}
// 	if (index != -1)
// 	{
// 		if (is_user_env_var(name))
// 			free(environ[index]);
// 		environ[index] = new_var;
// 	}
// 	else
// 	{
// 		count = 0;
// 		while (environ[count])
// 			count++;
// 		new_env = ft_realloc(environ, sizeof(char *) * (count + 2));
// 		if (!new_env)
// 			return (perror("realloc"), free(new_var), 1);
// 		environ = new_env;
// 		environ[count] = new_var;
// 		environ[count + 1] = NULL;
// 	}
// 	return (0);
// }