/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:45:12 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/29 13:02:50 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isoperator(int c)
{
	return (c == '|' || c == '<' || c == '>'
		|| c == '$' || c == '\'' || c == '\"');
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	skip_whites(char *line, int	*i)
{
	int	res;

	res = 0;
	while (line[*i] && ft_isspace(line[*i]))
		(*i)++;
	if (line[*i] != '\0')
		res = 1;
	return (res);
}

int	is_empty(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (1);
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

void	hanlde_quotes(char *str, int i, int *flag_single, int *flag_double)
{
	if ((str)[i] == '\'' && *flag_double == 0)
		*flag_single = !*flag_single;
	else if ((str)[i] == '"' && *flag_single == 0)
		*flag_double = !*flag_double;
}
