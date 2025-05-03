/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 11:31:52 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/03 11:35:59 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bva_newstr(char *str, int dollar_pos, t_var_info *var, char **myenvp)
{
	char	*dollar;
	char	*var_name;
	int		size;

	dollar = str + dollar_pos;
	size = get_var_name_size(dollar);
	if (size < 1)
		return (1);
	var_name = malloc(size + 1);
	if (!var_name)
		return (exit(1), 0);
	get_var_name(var_name, dollar);
	var->var_value = find_var_value(var_name, myenvp);
	var->suffix = ft_strdup(dollar + size);
	if (var->suffix == NULL)
	{
		perror("In bva_newstr function malloc failed");
		exit(EXIT_FAILURE);
	}
	free(var_name);
	return (0);
}

int	replace_str_with_var(char **str, t_var_info *var_data, char *new_str)
{
	free(*str);
	*str = new_str;
	if (var_data->var_value)
		free(var_data->var_value);
	free(var_data->prefix);
	free(var_data->suffix);
	return (1);
}

char	*build_new_string(t_var_info *var_data)
{
	int		new_str_len;
	char	*new_str;

	new_str_len = ft_strlen(var_data->prefix) + ft_strlen(var_data->suffix) + 1;
	if (var_data->var_value)
		new_str_len += ft_strlen(var_data->var_value);
	new_str = malloc(new_str_len);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, var_data->prefix, new_str_len);
	if (var_data->var_value)
		ft_strlcat(new_str, var_data->var_value, new_str_len);
	ft_strlcat(new_str, var_data->suffix, new_str_len);
	return (new_str);
}
