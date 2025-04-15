/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:27:27 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/15 12:08:04 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bridge_var_at(char **str, int dollar_pos)
{
	char	*dollar;
	int		size;
	char	*var_name;
	char	*var_value;
	char	*prefix;
	char 	*suffix;
	int		new_str_len;
	char	*new_str;

	dollar = *str + dollar_pos;
	size = get_var_name_size(dollar);
	// printf("size %d\n", size);
	if (size < 1)
		return (1);
	var_name = malloc(size + 1);
	if (!var_name)
		return (exit(1), 0);
	get_var_name(var_name, dollar);
	var_value = find_var_value(var_name);
	if (!var_value)
		var_value = "";
	prefix = ft_strndup(*str, dollar_pos);
	suffix = ft_strdup(dollar + size); // Не добавляем +1, т.к. за символом переменной может быть пробел
	new_str_len = ft_strlen(prefix) + ft_strlen(suffix) + 1;
	if (var_value)
		new_str_len += ft_strlen(var_value); // Учитываем длину значения переменной
	new_str = malloc(new_str_len);
	if (!new_str)
	{
		free(prefix);
		free(suffix);
		free(var_name);
		return (exit(1), 0);
	}
	ft_strlcpy(new_str, prefix, new_str_len);
	if (var_value)
		ft_strlcat(new_str, var_value, new_str_len);
	ft_strlcat(new_str, suffix, new_str_len);
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

	i = 0;
	flag_single = 0;
	flag_double = 0;
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

// int	bridge_var_at(char **str, int dollar_pos)
// {
// 	char	*dollar;
// 	int		size;
// 	char	*var_name;
// 	char	*var_value;
// 	char	*prefix;
// 	int		new_str_len;
// 	char	*new_str;

// 	dollar = *str + dollar_pos;
// 	size = get_var_name_size(dollar);
// 	// printf("size %d\n", size);
// 	if (size < 1)
// 		return (1);
// 	var_name = malloc(size + 1);
// 	if (!var_name)
// 		return (exit(1), 0);
// 	get_var_name(var_name, dollar);
// 	var_value = find_var_value(var_name);
// 	if (!var_value)
// 	{
// 		var_value = "";
// 		// var_value = malloc(2);
// 		// *var_value = ' ';
// 	}
// 	prefix = strndup(*str, dollar_pos);
// 	char *suffix = strdup(dollar + size); // Не добавляем +1, т.к. за символом переменной может быть пробел
// 	new_str_len = strlen(prefix) + strlen(suffix) + 1;
// 	if (var_value)
// 		new_str_len += strlen(var_value); // Учитываем длину значения переменной
// 	new_str = malloc(new_str_len);
// 	if (!new_str)
// 	{
// 		free(prefix);
// 		free(suffix);
// 		free(var_name);
// 		return (exit(1), 0);
// 	}
// 	strcpy(new_str, prefix);
// 	if (var_value)
// 		strcat(new_str, var_value);
// 	strcat(new_str, suffix);
// 	free(*str);
// 	*str = new_str;
// 	free(prefix);
// 	free(suffix);
// 	free(var_name);
// 	return (1);
// }
