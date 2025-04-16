/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_unset_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:51:40 by alex              #+#    #+#             */
/*   Updated: 2025/04/16 18:53:12 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern char	**environ;

int	handle_unset(char *line, int fd)
{
	char	*arg;

	arg = line + 6;
	if (arg && *arg != '\0')
		return (my_unsetenv(arg));
	write_stderr("unset: invalid syntax");
	return (1);
}

int	handle_export(char *line, int fd)
{
	char	**arr;
	char	*equals_pos;
	char	*value;
	char	*clean_line;

	if (ft_strlen(line) == 6 || check_line(line, 7))
		return (print_env(fd));
	arr = ft_split(line, ' ');
	if (!arr)
		return (1);
	clean_line = remove_quotes(arr[1]);
	if (!clean_line)
		return (free_arr(arr), 1);
	equals_pos = ft_strchr(clean_line, '=');
	if (equals_pos != NULL)
	{
		*equals_pos = '\0';
		value = equals_pos + 1;
	}
	else
		value = NULL;
	if (mysetenv(clean_line, value) == 1)
		return (free(clean_line), free_arr(arr), exit(1), 1);
	return (free(clean_line), free_arr(arr), 0);
}

int	allocate_var(char *name, char *value, char **new_var)
{
	int	len;

	len = ft_strlen(name);
	if (value)
		len += ft_strlen(value) + 1;
	*new_var = malloc(len + 1);
	if (!*new_var)
		return (perror("malloc"), 1);
	ft_strcpy(*new_var, name);
	if (value)
	{
		ft_strcat(*new_var, "=");
		ft_strcat(*new_var, value);
	}
	return (0);
}

int	mysetenv(char *name, char *value)
{
	int		index;
	char	*new_var;
	int		count;
	char	**new_env;

	index = find_var_in_env(name);
	if (allocate_var(name, value, &new_var))
		return (1);
	if (index != -1)
	{
		if (is_user_env_var(name))
			free(environ[index]);
		environ[index] = new_var;
	}
	else
	{
		count = ft_arrlen(environ);
		new_env = ft_realloc(environ, sizeof(char *) * (count + 2));
		printf("new_env: allocated at %p\n", new_env);
		if (!new_env)
			return (perror("realloc"), free(new_var), 1);
		environ = new_env;
		environ[count] = new_var;
		environ[count + 1] = NULL;
	}
	return (0);
}
