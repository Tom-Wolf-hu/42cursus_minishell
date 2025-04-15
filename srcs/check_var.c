/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:14:33 by alex              #+#    #+#             */
/*   Updated: 2025/04/15 18:57:18 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char	**environ;

int	cmp_names(char *name1, char *name2)
{
	int	i;

	i = 0;
	while (name1[i] && name2[i] && name1[i] != '=' && name2[i] != '=')
	{
		if (name1[i] != name2[i])
			return (0);
		i++;
	}
	if ((name1[i] == '=' && name2[i] == '\0')
		|| (name1[i] == '\0' && name2[i] == '\0'))
		return (1);
	return (0);
}

char	*find_var_value(char *var_name)
{
	int		i;
	char	*value;

	i = 0;
	while (environ[i])
	{
		if (cmp_names(environ[i], var_name))
		{
			value = environ[i];
			while (*value && *value != '=')
				value++;
			if (*value == '=')
				return (value + 1);
		}
		i++;
	}
	return (NULL);
}

int	get_var_name_size(char *str)
{
	int	i;

	if (!str || str[0] != '$')
		return (0);
	i = 1;
	if (str[i] == '\0' || str[i] == ' ' || str[i] == '$' || str[i] == '='
		|| str[i] == '\'' || str[i] == '"' || str[i] == '\t')
		return (0);
	while (str[i] && str[i] != ' ' && str[i] != '$' && str[i] != '='
		&& str[i] != '\'' && str[i] != '"' && str[i] != '\t' && str[i] != '|')
		i++;
	return (i);
}

void	get_var_name(char *dest, char *str)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (str[i] && str[i] != ' ' && str[i] != '$' && str[i] != '='
		&& str[i] != '\'' && str[i] != '"' && str[i] != '\t' && str[i] != '|')
	{
		dest[j++] = str[i++];
	}
	dest[j] = '\0';
}

void	change_str(char **str, char *name, char *value)
{
	char	*start;
	int		prefix_len;
	int		name_len;
	int		value_len;
	char	*result;

	start = strstr(*str, "$");
	if (!start)
		return ;
	prefix_len = start - *str;
	name_len = strlen(name);
	value_len = strlen(value);
	result = malloc(prefix_len + value_len + strlen(start + name_len + 1) + 1);
	if (!result)
		return ;
	strncpy(result, *str, prefix_len);
	result[prefix_len] = '\0';
	strcat(result, value);
	strcat(result, start + name_len + 1);
	free(*str);
	*str = result;
}

void	remove_var_name(char **str, char *name)
{
	char	*start;
	int		prefix_len;
	int		name_len;
	char	*result;

	start = strstr(*str, "$");
	if (!start)
		return ;
	prefix_len = start - *str;
	name_len = strlen(name);
	result = malloc(strlen(*str) - name_len + 1);
	if (!result)
		return ;
	strncpy(result, *str, prefix_len);
	result[prefix_len] = '\0';
	strcat(result, start + name_len + 1);
	free(*str);
	*str = result;
}

int	bridge_var_at(char **str, int dollar_pos)
{
	char	*dollar;
	char	*var_name;
	char	*var_value;
	char	*prefix;
	char	*suffix;
	int		new_str_len;
	char	*new_str;
	int		size;

	dollar = *str + dollar_pos;
	size = get_var_name_size(dollar);
	if (size < 1)
		return (1);
	var_name = malloc(size + 1);
	if (!var_name)
		return (exit(1), 0);
	get_var_name(var_name, dollar);
	var_value = find_var_value(var_name);
	if (!var_value)
		var_value = "";
	prefix = strndup(*str, dollar_pos);
	suffix = strdup(dollar + size);
	new_str_len = strlen(prefix) + strlen(suffix) + 1;
	if (var_value)
		new_str_len += strlen(var_value);
	new_str = malloc(new_str_len);
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
	return (1);
}

void	bridge_var(char **str)
{
	int	i;
	int	flag_single;
	int	flag_double;

	flag_single = 0;
	flag_double = 0;
	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '\'' && flag_double == 0)
			flag_single = !flag_single;
		else if ((*str)[i] == '"' && flag_single == 0)
			flag_double = !flag_double;
		if ((*str)[i] == '$' && flag_single == 0)
		{
			if (!bridge_var_at(str, i))
				break ;
			i = -1;
			flag_single = 0;
			flag_double = 0;
		}
		i++;
	}
}
