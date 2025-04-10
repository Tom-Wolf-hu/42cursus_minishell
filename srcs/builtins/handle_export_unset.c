/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_unset.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:49:13 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/10 15:03:52 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

extern char **environ; // Массив переменных окружения
char **user_env_vars = NULL;
int user_env_size = 0;

int find_var_in_env(char *name)
{
    int i;

	i = 0;
    while (environ[i])
	{
		// if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0 && environ[i][ft_strlen(name)] == '=')
		if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0)
            return (i);
        i++;
    }
    return (-1);
}

int add_user_env_var(char *name)
{
    int i = 0;
    while (i < user_env_size)
    {
        if (strncmp(user_env_vars[i], name, strlen(name)) == 0 &&
            user_env_vars[i][strlen(name)] == '\0')
            return 0; // Уже добавлено
        i++;
    }

    char **new_list = realloc(user_env_vars, sizeof(char *) * (user_env_size + 1));
    if (!new_list)
        return (perror("realloc user_env_vars"), 1);

    user_env_vars = new_list;
    user_env_vars[user_env_size] = strdup(name); // только имя, без значения
    user_env_size++;
    return 0;
}

int is_user_env_var(char *name)
{
    for (int i = 0; i < user_env_size; i++)
    {
        if (strncmp(user_env_vars[i], name, strlen(name)) == 0 &&
            user_env_vars[i][strlen(name)] == '\0')
            return 1;
    }
    return 0;
}

int mysetenv(char *name, char *value)
{
	int		index;
	char	*new_var;

	index = find_var_in_env(name);
	// if (index != -1)
	// 	free(environ[index]);

	// Если value == NULL, не добавляем '='
	if (value)
		new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	else
		new_var = malloc(ft_strlen(name) + 1);
	if (!new_var)
		return (exit(1), 1);
	if (new_var == NULL)
		return (perror("malloc"), 1);
	strcpy(new_var, name);
	if (value)
	{
		strcat(new_var, "=");
		strcat(new_var, value);
	}
	if (index != -1)
	{
		if (is_user_env_var(name))
			free(environ[index]);
		environ[index] = new_var;
	}
	else
	{
		int count = 0;
		while (environ[count])
			count++;

		// environ = realloc(environ, sizeof(char*) * (count + 2));
		char **new_env = realloc(environ, sizeof(char *) * (count + 2));
		if (!new_env)
			return (perror("realloc"), free(new_var), 1);
		environ = new_env;
		environ[count] = new_var;
		environ[count + 1] = NULL;
	}
	return (0);
}

void	free_var_after_exit()
{
	int	i;

	i = 0;
	while (environ[i])
	{
		if (is_user_env_var(environ[i]))
			free(environ[i]);
		i++;
	}
	free_arr(user_env_vars);
}

int	handle_export(char *line, int fd)
{
	char	*arg;
	char	**arr;
	char	*equals_pos;
	char	*value;
	char	*clean_line;

	if (ft_strlen(line) == 6 || check_line(line, 7))
		return (print_env(fd));
	else
	{
		arr = ft_split(line, ' ');
		if (!arr)
			return (1);
		clean_line = remove_quotes(arr[1]);
		if (!clean_line)
			return (free_arr(arr), 1);
		arg = clean_line;
		equals_pos = ft_strchr(arg, '=');
		if (equals_pos != NULL)
		{
			*equals_pos = '\0';
			value = equals_pos + 1;
		}
		else
			value = NULL;
		if (mysetenv(arg, value) == 1)
			return (free(clean_line), free_arr(arr), exit(1), 1);
		}
	free(clean_line);
	free_arr(arr);
	return (0);
}

int my_unsetenv(char *name)
{
	int index;

	index = find_var_in_env(name);
	if (index != -1)
	{
		// free(environ[index]);
		if (is_user_env_var(name))
			free(environ[index]);
		while (environ[index])
		{
			environ[index] = environ[index + 1];
			index++;
		}
		char **new_env = realloc(environ, sizeof(char*) * (index));
		if (!new_env && index > 0)
			return (perror("realloc"), exit(1), 1);
		environ = new_env;
	}
	return (0);
}

int	handle_unset(char *line, int fd)
{
	char	*arg;

	arg = line + 6;
	if (arg && *arg != '\0')
		return (my_unsetenv(arg));
	// printf("unset: invalid syntax\n");
	write(fd, "unset: invalid syntax\n", ft_strlen("unset: invalid syntax\n"));
	return (1);
}
