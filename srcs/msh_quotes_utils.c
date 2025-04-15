/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:36:27 by alex              #+#    #+#             */
/*   Updated: 2025/04/15 18:39:36 by alex             ###   ########.fr       */
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

char	*copy_quoted_word(char *line, char *result, int i)
{
	int	j;

	j = 0;
	while (line[i] && line[i] != '\'' && line[i] != '\"')
	{
		if (ft_isspace(line[i]))
			return (free(result), NULL);
		result[j++] = line[i++];
	}
	if (line[i] == '\'' || line[i] == '\"')
		i++;
	while (line[i])
		result[j++] = line[i++];
	result[j] = '\0';
	return (result);
}

char	*remove_quotes_first_word(char *line)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] != '\'' && line[i] != '\"')
		return (ft_strdup(line));
	result = malloc(strlen(line) - 2 + 1);
	if (!result)
		return (NULL);
	i++;
	if (ft_isspace(line[i]))
		return (NULL);
	return (copy_quoted_word(line, result, i));
}

void	process_quotes_loop(char *line, char *result)
{
	int		i;
	int		pos;
	char	*tmp;

	i = 0;
	pos = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			tmp = get_temp_remove_quotes(line, &i, line[i]);
			if (tmp)
			{
				ft_strcpy(result + pos, tmp);
				pos += ft_strlen(tmp);
				free(tmp);
			}
		}
		else
			result[pos++] = line[i++];
	}
	result[pos] = '\0';
}

char	*remove_quotes(char *line)
{
	int		i;
	int		len;
	char	*result;
	int		pos;
	char	*tmp;

	i = 0;
	len = ft_strlen(line);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	pos = 0;
	process_quotes_loop(line, result);
	return (result);
}
