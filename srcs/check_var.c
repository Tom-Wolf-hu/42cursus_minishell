/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:14:33 by alex              #+#    #+#             */
/*   Updated: 2025/04/30 19:52:49 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	cmp_names(char *env_str, char *name)
{
	int	i;

	i = 0;
	while (env_str[i] && name[i] && env_str[i] != '=' && name[i] != '=')
	{
		if (env_str[i] != name[i])
			return (0);
		i++;
	}
	return (env_str[i] == '=' && name[i] == '\0');
}

char	*find_var_value(char *var_name, char **myenvp)
{
	int		i;
	int		j;

	i = 0;
	while (myenvp[i])
	{
		if (cmp_names(myenvp[i], var_name))
		{
			j = 0;
			while (myenvp[i][j] && myenvp[i][j] != '=')
				j++;
			return (ft_strdup(myenvp[i] + j + 1));
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
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

void	get_var_name(char *dest, char *str)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		dest[j++] = str[i++];
	}
	dest[j] = '\0';
}

int	handle_dollar(char **str, t_var_info *var_data,
	int dollar_pos, char *dollar)
{
	char	*pid_str;
	int		new_str_len;
	char	*new_str;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (0);
	var_data->prefix = ft_strndup(*str, dollar_pos);
	var_data->suffix = ft_strdup(dollar + 2);
	new_str_len = ft_strlen(var_data->prefix)
		+ ft_strlen(pid_str) + ft_strlen(var_data->suffix) + 1;
	new_str = malloc(new_str_len);
	if (!new_str)
	{
		return (free(var_data->prefix), free(var_data->suffix),
			free(pid_str), exit(1), 0);
	}
	ft_strlcpy(new_str, var_data->prefix, new_str_len);
	ft_strlcat(new_str, pid_str, new_str_len);
	ft_strlcat(new_str, var_data->suffix, new_str_len);
	free(*str);
	*str = new_str;
	return (free(var_data->prefix), free(var_data->suffix), free(pid_str), 1);
}
