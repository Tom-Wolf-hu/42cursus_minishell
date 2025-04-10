/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:14:33 by alex              #+#    #+#             */
/*   Updated: 2025/04/10 16:43:16 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char **environ;

int cmp_names(char *name1, char *name2)
{
    int i = 0;

    while (name1[i] && name2[i] && name1[i] != '=' && name2[i] != '=')
    {
        if (name1[i] != name2[i])
            return 0;
        i++;
    }
    if ((name1[i] == '=' && name2[i] == '\0') || (name1[i] == '\0' && name2[i] == '\0'))
        return 1;
    return 0;
}

char *find_var_value(char *var_name)
{
    int i = 0;
    char *value;

    while (environ[i])
    {
        if (cmp_names(environ[i], var_name))
        {
            value = environ[i];
            while (*value && *value != '=')
                value++;
            if (*value == '=')
                return value + 1;
        }
        i++;
    }
    return NULL;
}

int get_var_name_size(char *str)
{
    int i = 0;

    if (!str || str[0] != '$')
        return 0;

    i = 1;
    if (str[i] == '\0' || str[i] == ' ' || str[i] == '$' || str[i] == '=' ||
        str[i] == '\'' || str[i] == '"' || str[i] == '\t')
        return 0;

    while (str[i] && str[i] != ' ' && str[i] != '$' && str[i] != '=' &&
        str[i] != '\'' && str[i] != '"' && str[i] != '\t' && str[i] != '|')
        i++;

    return i; // Убираем -1, потому что $ не включается в имя
}

void get_var_name(char *dest, char *str)
{
    int i = 1;
    int j = 0;

    while (str[i] && str[i] != ' ' && str[i] != '$' && str[i] != '=' &&
        str[i] != '\'' && str[i] != '"' && str[i] != '\t' && str[i] != '|')
    {
        dest[j++] = str[i++];
    }
    dest[j] = '\0';
}

void change_str(char **str, char *name, char *value)
{
    char *start = strstr(*str, "$");
    if (!start) return;

    int prefix_len = start - *str;
    int name_len = strlen(name);
    int value_len = strlen(value);
    int total_len = prefix_len + value_len + strlen(start + name_len + 1) + 1;

    char *result = malloc(total_len);
    if (!result)
        return;

    strncpy(result, *str, prefix_len);
    result[prefix_len] = '\0';
    strcat(result, value);
    strcat(result, start + name_len + 1);

    free(*str);
    *str = result;
}

void remove_var_name(char **str, char *name)
{
    char *start = strstr(*str, "$");
    if (!start)
		return;
    int prefix_len = start - *str;
    int name_len = strlen(name);
    char *result = malloc(strlen(*str) - name_len + 1);
    if (!result)
		return;
    strncpy(result, *str, prefix_len);
    result[prefix_len] = '\0';
    strcat(result, start + name_len + 1);
    free(*str);
    *str = result;
}

int bridge_var_at(char **str, int dollar_pos)
{
    char *dollar = *str + dollar_pos;
    int size = get_var_name_size(dollar);
	// printf("size %d\n", size);
    if (size < 1)
        return 1;
    char *var_name = malloc(size + 1);
    if (!var_name)
        return (exit(1), 0);
    get_var_name(var_name, dollar);
    char *var_value = find_var_value(var_name);
	if (!var_value)
	{
		var_value = "";
		// var_value = malloc(2);
		// *var_value = ' ';
	}
    char *prefix = strndup(*str, dollar_pos);
    char *suffix = strdup(dollar + size); // Не добавляем +1, т.к. за символом переменной может быть пробел
    int new_str_len = strlen(prefix) + strlen(suffix) + 1;
    if (var_value)
        new_str_len += strlen(var_value);  // Учитываем длину значения переменной
    char *new_str = malloc(new_str_len);
    if (!new_str)
	{
        free(prefix);
        free(suffix);
        free(var_name);
        return (exit(1), 0);
    }
    strcpy(new_str, prefix);
    if (var_value)
        strcat(new_str, var_value);
    strcat(new_str, suffix);
    free(*str);
    *str = new_str;
    free(prefix);
    free(suffix);
    free(var_name);
	return 1;
}

void bridge_var(char **str)
{
    int i = 0;
    int flag_single = 0;
    int flag_double = 0;

    while ((*str)[i])
    {
        if ((*str)[i] == '\'' && flag_double == 0)
            flag_single = !flag_single;
        else if ((*str)[i] == '"' && flag_single == 0)
            flag_double = !flag_double;
        if ((*str)[i] == '$' && flag_single == 0)
        {
            if (!bridge_var_at(str, i))
				break;
            i = -1;
            flag_single = 0;
            flag_double = 0;
        }
        i++;
    }
}
