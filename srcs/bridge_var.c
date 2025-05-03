/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:27:27 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/03 11:36:16 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	bridge_var_at(char **str, int dollar_pos, char **myenvp)
{
	t_var_info	var_data;
	char		*dollar;
	char		*new_str;

	dollar = *str + dollar_pos;
	if (dollar[1] == '$')
		return (handle_dollar(str, &var_data, dollar_pos, dollar));
	var_data.var_value = NULL;
	if (bva_newstr(*str, dollar_pos, &var_data, myenvp) == 1)
		return (0);
	var_data.prefix = ft_strndup(*str, dollar_pos);
	if (!var_data.prefix)
		return (free(var_data.suffix), exit(1), 0);
	if (!var_data.var_value && ft_strlen(var_data.prefix) == 0
		&& ft_strlen(var_data.suffix) == 0)
	{
		free(*str);
		*str = NULL;
		return (free(var_data.prefix), free(var_data.suffix), 0);
	}
	new_str = build_new_string(&var_data);
	if (!new_str)
		return (free(var_data.prefix), free(var_data.suffix),
			free(var_data.var_value), exit(1), 0);
	return (replace_str_with_var(str, &var_data, new_str));
}

void	i_and_flags_set_zero(int *i, int *flag_single, int *flag_double)
{
	*i = 0;
	*flag_single = 0;
	*flag_double = 0;
}

void	bridge_var(char **str, char **myenvp)
{
	int	i;
	int	flag_single;
	int	flag_double;

	i_and_flags_set_zero(&i, &flag_single, &flag_double);
	while ((*str)[i])
	{
		hanlde_quotes(*str, i, &flag_single, &flag_double);
		if ((*str)[i] == '$' && flag_single == 0)
		{
			if (!bridge_var_at(str, i, myenvp))
			{
				if (!*str)
					return ;
				i++;
				continue ;
			}
			i_and_flags_set_zero(&i, &flag_single, &flag_double);
			continue ;
		}
		i++;
	}
}

// void	bridge_var(char **str, char **myenvp)
// {
// 	int	i;
// 	int	flag_single;
// 	int	flag_double;

// 	i = 0;
// 	flag_single = 0;
// 	flag_double = 0;
// 	while ((*str)[i])
// 	{
// 		hanlde_quotes(*str, i, &flag_single, &flag_double);
// 		if ((*str)[i] == '$' && flag_single == 0)
// 		{
// 			if (!bridge_var_at(str, i, myenvp))
// 			{
// 				if (!*str)
// 					return ;
// 				i++;
// 				continue ;
// 			}
// 			i = 0;
// 			flag_single = 0;
// 			flag_double = 0;
// 			continue ;
// 		}
// 		i++;
// 	}
// }
