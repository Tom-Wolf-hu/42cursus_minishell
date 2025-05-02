/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:41:53 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/02 16:58:07 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_command_quotes(char *line)
{
	int	in_quote;
	int	i;

	i = 0;
	in_quote = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if ((line[i] == '\'' || line[i] == '\"') && line[i + 1] == line[i])
		return (0);
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && !in_quote)
			in_quote = 1;
		else if ((line[i] == '\'' || line[i] == '\"') && in_quote)
			in_quote = 0;
		if (ft_isspace(line[i]) && in_quote)
			return (0);
		i++;
	}
	if (in_quote)
		return (0);
	return (1);
}

int	check_quotes_arg(char *line)
{
	int		i;
	char	first_quote;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == '\'' || line[i] == '\"')
	{
		first_quote = line[i];
		i++;
	}
	while (line[i] && first_quote)
	{
		if (ft_isspace(line[i]))
			return (0);
		if (line[i] == first_quote)
			return (1);
		i++;
	}
	return (0);
}

int	check_empty_cmd(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == '\'' || line[i] == '\"')
	{
		quote = line[i];
		i++;
	}
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] == quote || line[i] == '\0')
		return (0);
	return (1);
}
