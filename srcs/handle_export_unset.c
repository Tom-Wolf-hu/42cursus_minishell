/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_unset.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:49:13 by omalovic          #+#    #+#             */
/*   Updated: 2025/03/03 15:11:03 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char **environ; // Массив переменных окружения

int find_var_in_env(char *name)
{
    int i;

	i = 0;
    while (environ[i])
	{
        if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0 && environ[i][ft_strlen(name)] == '=')
            return (i);
        i++;
    }
    return (-1);
}

int mysetenv(char *name, char *value)
{
	int index;
	char *new_var;

	index = find_var_in_env(name);
	if (index != -1)
	{
		free(environ[index]);
		new_var = malloc(strlen(name) + strlen(value) + 2);
		if (new_var == NULL)
			return (perror("malloc"), 1);
		strcpy(new_var, name); // Копируем имя переменной
		strcat(new_var, "=");  // Добавляем знак '='
		strcat(new_var, value); // Добавляем значение переменной
		environ[index] = new_var; // Обновляем переменную окружения
	}
	else
	{
		// Если переменной нет, добавим её в окружение
		int count = 0;
		while (environ[count]) count++; // Считаем количество переменных в окружении
		environ = realloc(environ, sizeof(char*) * (count + 2)); // Выделяем место для новой переменной
		if (environ == NULL) {
			perror("realloc");
			return (1);
		}
		
		new_var = malloc(strlen(name) + strlen(value) + 2); // +2 для '=' и '\0'
		if (new_var == NULL) {
			perror("malloc");
			return (1);
		}
		
		// Строку создаём вручную
		strcpy(new_var, name);  // Копируем имя переменной
		strcat(new_var, "=");   // Добавляем знак '='
		strcat(new_var, value); // Добавляем значение переменной
		
		environ[count] = new_var; // Добавляем в окружение
		environ[count + 1] = NULL; // Обязательно завершаем массив NULL
	}
	return (0);
}

int	handle_export(char *line, int fd)
{
	char	*arg;
	char	*equals_pos;
	char	*value;

	if (ft_strlen(line) == 6 || check_line(line, 7))
		return (print_env(fd));
	else
	{
		arg = line + 7;
		equals_pos = ft_strchr(arg, '=');
		if (equals_pos != NULL)
		{
			*equals_pos = '\0';
			value = equals_pos + 1;
			return (mysetenv(arg, value));
		}
		else
			return (write(fd, "export: invalid syntax\n", ft_strlen("export: invalid syntax\n")), 1);
	}
	return (0);
}

int my_unsetenv(char *name)
{
	int index;

	index = find_var_in_env(name);
	if (index != -1)
	{
		free(environ[index]);
		while (environ[index])
		{
			environ[index] = environ[index + 1];
			index++;
		}
		environ = realloc(environ, sizeof(char*) * (index)); 
		if (environ == NULL && index > 0)
		{
			perror("realloc");
			return (1);
		}
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
