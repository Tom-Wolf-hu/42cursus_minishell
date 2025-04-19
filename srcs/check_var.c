/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:14:33 by alex              #+#    #+#             */
/*   Updated: 2025/04/18 21:28:39 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*find_var_value(char *var_name, char **myenvp)
{
	int		i;
	char	*value;

	i = 0;
	while (myenvp[i])
	{
		if (cmp_names(myenvp[i], var_name))
		{
			value = myenvp[i];
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
		&& str[i] != '\'' && str[i] != '"' && str[i] != '\t' && str[i] != '|'
		&& str[i] != ',' && str[i] != '.')
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
		&& str[i] != '\'' && str[i] != '"' && str[i] != '\t' && str[i] != '|'
		&& str[i] != ',' && str[i] != '.')
	{
		dest[j++] = str[i++];
	}
	dest[j] = '\0';
}
