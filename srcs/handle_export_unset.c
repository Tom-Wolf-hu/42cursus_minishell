/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_unset.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:49:13 by omalovic          #+#    #+#             */
/*   Updated: 2025/02/19 20:30:30 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char **environ; // Массив переменных окружения

int find_var_in_env(char *name)
{
    int i = 0;

    while (environ[i])
	{
        if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0 && environ[i][ft_strlen(name)] == '=')
            return (i);
        i++;
    }
    return (-1);
}

// Функция для установки переменной в окружении
void mysetenv(char *name, char *value)
{
    int index = find_var_in_env(name);
    char *new_var;
    
    // Если переменная уже существует, обновим её
    if (index != -1)
	{
        free(environ[index]);  // Освободить старое значение
        new_var = malloc(strlen(name) + strlen(value) + 2); // +2 для '=' и '\0'
        if (new_var == NULL)
            return (perror("malloc"));
        // Строку создаём вручную, без использования sprintf
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
            return;
        }
        
        new_var = malloc(strlen(name) + strlen(value) + 2); // +2 для '=' и '\0'
        if (new_var == NULL) {
            perror("malloc");
            return;
        }
        
        // Строку создаём вручную
        strcpy(new_var, name);  // Копируем имя переменной
        strcat(new_var, "=");   // Добавляем знак '='
        strcat(new_var, value); // Добавляем значение переменной
        
        environ[count] = new_var; // Добавляем в окружение
        environ[count + 1] = NULL; // Обязательно завершаем массив NULL
    }
}

void	handle_export(char *line)
{
	char	*arg;
	char	*equals_pos;
	char	*value;

	if (ft_strlen(line) == 6 || check_line(line, 7))
		print_env();
	else
	{
		arg = line + 7;
		equals_pos = ft_strchr(arg, '=');
		if (equals_pos != NULL)
		{
			*equals_pos = '\0';
			value = equals_pos + 1;
			mysetenv(arg, value);
		}
		else
			printf("export: invalid syntax\n");
	}
}

// Функция для удаления переменной из окружения
void my_unsetenv(char *name) {
    int index = find_var_in_env(name);
    
    // Если переменная найдена
    if (index != -1) {
        free(environ[index]); // Освобождаем память, занятую переменной

        // Сдвигаем все последующие элементы на одну позицию влево
        while (environ[index]) {
            environ[index] = environ[index + 1];
            index++;
        }

        // Обновляем массив окружения, уменьшив его размер на 1
        environ = realloc(environ, sizeof(char*) * (index)); 
        if (environ == NULL && index > 0) {
            perror("realloc");
            return;
        }
    }
}

void	handle_unset(char *line)
{
	char	*arg;

	arg = line + 6;
	if (arg && *arg != '\0') {
        my_unsetenv(arg);
    } else {
        printf("unset: invalid syntax\n");
    }
}
