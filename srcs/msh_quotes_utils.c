/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:36:27 by alex              #+#    #+#             */
/*   Updated: 2025/05/02 17:35:16 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_first_quotes(char *line)
{
	int	i;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] != '\'' && line[i] != '\"')
		return (1);
	return (0);
}

char	*copy_str(char **result, char *line)
{
	int	first_quote;
	int	i;
	int	j;

	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	i++;
	j = 0;
	first_quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && !first_quote)
		{
			first_quote = 1;
			i++;
			continue ;
		}
		(*result)[j] = line[i];
		i++;
		j++;
	}
	(*result)[j] = '\0';
	return ((*result));
}

char	*remove_quotes_first_word(char *line)
{
	char	*result;

	if (no_first_quotes(line))
		return (ft_strdup(line));
	if (!check_quotes_arg(line) || !check_empty_cmd(line))
		return (NULL);
	result = malloc(ft_strlen(line) - 2 + 1);
	if (!result)
		return (NULL);
	return (copy_str(&result, line));
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

	i = 0;
	len = ft_strlen(line);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	pos = 0;
	process_quotes_loop(line, result);
	return (result);
}
