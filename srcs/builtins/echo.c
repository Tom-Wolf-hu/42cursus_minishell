/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:40:45 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/02 12:51:30 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *remove_quotes(char *str)
{
    int i = 0, j = 0;
    int len = strlen(str);
    char *new_str = malloc(len + 1);

    if (!new_str)
        return NULL;

    int inside_quotes = 0;

    while (str[i])
    {
        // Если мы находим кавычки, меняем состояние
        if ((str[i] == '"' || str[i] == '\'') && !inside_quotes)
        {
            inside_quotes = 1;
            i++;
            continue;
        }
        if ((str[i] == '"' || str[i] == '\'') && inside_quotes)
        {
            inside_quotes = 0;
            i++;
            continue;
        }

        // Если мы не внутри кавычек, копируем символ в новый массив
        if (!inside_quotes)
            new_str[j++] = str[i];
        i++;
    }

    new_str[j] = '\0';
    return new_str;
}

void mywrite(char *line, int fd)
{
    int i = 0;
    int len = strlen(line);
    
    while (i < len)
    {
        write(fd, &line[i], 1);
        i++;
    }
}

int handle_echo(char *line, int fd)
{
    char **arr;
    int i = 0;

    arr = ft_split(line, ' ');
    if (!arr)
        return (free(line), exit(1), 1);

    while (arr[i])
        i++;

    // Если передана только пустая строка
    if (i == 1)
    {
        write(fd, "\n", 1);
        free_arr(arr);
        return 0;
    }

    // Если флаг "-n" найден
    if (i == 2 && ft_strcmp(arr[1], "-n") == 0)
    {
        free_arr(arr);
        return 0;
    }

    // Если более двух аргументов, проверяем флаг "-n"
    if (i > 2 && ft_strcmp(arr[1], "-n") == 0)
    {
        // Обрабатываем строки начиная с третьего элемента
        for (int j = 2; j < i; j++)
        {
            char *cleaned_str = remove_quotes(arr[j]);
            mywrite(cleaned_str, fd);
            free(cleaned_str);
            if (j < i - 1)
                write(fd, " ", 1);
        }
        free_arr(arr);
        return 0;
    }

    // Если нет флага "-n", просто выводим все строки
    for (int j = 1; j < i; j++)
    {
        char *cleaned_str = remove_quotes(arr[j]);
        mywrite(cleaned_str, fd);
        free(cleaned_str);
        if (j < i - 1)
            write(fd, " ", 1);
    }

    write(fd, "\n", 1);
    free_arr(arr);
    return 0;
}

// int main()
// {
// 	handle_echo("echo \"'hello'\"", 1);
// }