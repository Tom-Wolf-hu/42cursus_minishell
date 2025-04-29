/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:27:27 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/29 13:02:43 by omalovic         ###   ########.fr       */
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
	new_str = build_new_string(&var_data);
	if (!new_str)
		return (free(var_data.prefix), free(var_data.suffix),
			free(var_data.var_value), exit(1), 0);
	return (replace_str_with_var(str, &var_data, new_str));
}

void	bridge_var(char **str, char **myenvp)
{
	int	i;
	int	flag_single;
	int	flag_double;

	i = 0;
	flag_single = 0;
	flag_double = 0;
	while ((*str)[i])
	{
		hanlde_quotes(*str, i, &flag_single, &flag_double);
		if ((*str)[i] == '$' && flag_single == 0)
		{
			if (!bridge_var_at(str, i, myenvp))
			{
				i++;
				continue ;
			}
			i = 0;
			flag_single = 0;
			flag_double = 0;
			continue ;
		}
		i++;
	}
}

//  echo "$USER $$ $$$ $PATH" $USER