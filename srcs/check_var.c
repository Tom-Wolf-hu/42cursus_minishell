/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:14:33 by alex              #+#    #+#             */
/*   Updated: 2025/04/15 10:28:24 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char **environ;

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
	if ((name1[i] == '=' && name2[i] == '\0') || (name1[i] == '\0'
			&& name2[i] == '\0'))
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

	i = 0;
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

/*
    The following two function we don't need any more
    After we discussed, we can remove
*/

void	change_str(char **str, char *name, char *value)
{
	char	*start;
	int		prefix_len;
	int		name_len;
	int		value_len;
	int		total_len;
	char	*result;

	start = strstr(*str, "$");
	if (!start)
		return ;
	prefix_len = start - *str;
	name_len = strlen(name);
	value_len = strlen(value);
	total_len = prefix_len + value_len + strlen(start + name_len + 1) + 1;
	result = malloc(total_len);
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
