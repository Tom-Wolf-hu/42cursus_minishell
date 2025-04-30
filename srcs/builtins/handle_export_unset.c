/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_unset.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:49:13 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/30 19:20:52 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	full_arr(int dlt_num, int size, char **result, char ***myenvp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < size)
	{
		if (i == dlt_num)
		{
			i++;
			continue ;
		}
		result[j] = ft_strdup((*myenvp)[i++]);
		if (!result[j])
			return (free_arr(result));
		j++;
	}
	result[j] = NULL;
}

char	**my_unsetenv(char *name, char ***myenvp)
{
	int		size;
	char	**result;
	int		dlt_num;

	dlt_num = find_var_in_env(name, *myenvp);
	if (dlt_num == -1)
		return (NULL);
	size = ft_arrlen(*myenvp);
	result = malloc(sizeof(char *) * size);
	if (!result)
		return (NULL);
	full_arr(dlt_num, size, result, myenvp);
	return (result);
}

char	**create_new_arr(char *str, char **myenvp)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (ft_arrlen(myenvp) + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (myenvp[i])
	{
		new_env[i] = ft_strdup(myenvp[i]);
		if (!new_env[i])
			return (free_arr(new_env), NULL);
		i++;
	}
	new_env[i] = ft_strdup(str);
	i++;
	new_env[i] = NULL;
	return (new_env);
}

int	mysetenv(char *name, char *value, char ***myenvp)
{
	char	**new_env;
	char	*new_str;
	int		index;

	index = find_var_in_env(name, *myenvp);
	if (value)
		new_str = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	else
		new_str = malloc(ft_strlen(name) + 1);
	if (!new_str)
		return (1);
	new_str[0] = '\0';
	ft_strcat(new_str, name);
	if (value)
	{
		ft_strcat(new_str, "=");
		ft_strcat(new_str, value);
	}
	if (index != -1)
		return (free((*myenvp)[index]), (*myenvp)[index] = new_str, 0);
	new_env = create_new_arr(new_str, *myenvp);
	if (!new_env)
		return (free(new_str), 1);
	return (free_arr(*myenvp), *myenvp = new_env, free(new_str), 0);
}

int	is_valid_identifier(const char *str)
{
	if (!str || !(*str))
		return (0);
	if (!(ft_isalpha(*str) || *str == '_'))
		return (0);
	while (*str && *str != '=')
	{
		if (!(ft_isalnum(*str) || *str == '_'))
			return (0);
		str++;
	}
	return (1);
}
