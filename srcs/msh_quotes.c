/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:25:56 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/15 17:08:09 by alex             ###   ########.fr       */
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

char	*remove_quotes_first_word(char *line)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	if (!line)
		return (NULL);
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != '\'' && line[i] != '\"')
		return (strdup(line));
	result = malloc(strlen(line) - 2 + 1);
	if (!result)
		return (NULL);
	i++;
	if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
		return (NULL);
	while (line[i] && line[i] != '\'' && line[i] != '\"')
	{
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
			return (free(result), NULL);
		result[j] = line[i];
		j++;
		i++;
	}
	if (line[i] == '\'' || line[i] == '\"')
		i++;
	while (line[i])
	{
		result[j] = line[i];
		j++;
		i++;
	}
	result[j] = '\0';
	return (result);
}

char	*get_temp_remove_quotes(char *line, int *i, char quotes)
{
	int		len;
	int		start;
	int		end;
	char	*result;

	len = ft_strlen(line);
	start = *i + 1;
	end = start;
	while (end < len)
	{
		if (line[end] == '\\' && quotes == '\"' && end + 1 < len && (quotes == '\"' || quotes == '\''))
			end += 2;
		else if (line[end] == quotes)
			break ;
		else
			end++;
	}
	result = (char *)malloc((end - start + 1) * sizeof(char));
	if (!result)
		return (NULL);
	ft_memcpy(result, line + start, end - start);
	result[end - start] = '\0';
	if (end < len && line[end] == quotes)
		*i = end + 1;
	else
		*i = end;
	return (result);
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
	while (i < len)
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
	return (result);
}

void	write_stderr(char *str)
{
	int	strlen;

	if (!str)
		str = "Error occured.\n";
	strlen = ft_strlen(str);
	write(2, str, strlen);
	write(2, "\n", 1);
}

int	check_quotes(char *line)
{
	int	i;
	int	j;
	int	end_q;

	i = 0;
	end_q = 1;
	while (line[i])
	{
		if (line[i] == ';' || line[i] == '\\')
			return (write_stderr("The character is not supported"), 1);
		if (line[i] == '\'' || line[i] == '\"')
		{
			j = i;
			i++;
			end_q = 0;
			while (line[i])
			{
				if (line[i] == '\\' && line[j] == '\"')
					return (write_stderr("The character is not supported"), 1);
				if (line[i] == line[j])
				{
					end_q = 1;
					break;
				}
				i++;
			}
		}
		i++;
	}
	if (end_q == 0)
	{
		write_stderr("The quotes are not closed");
		return (1);
	}
	return (0);
}
