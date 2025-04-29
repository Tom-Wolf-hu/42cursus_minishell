/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:25:56 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/29 02:22:22 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_quote_end(char *line, int start, int len, char quotes)
{
	int	end;

	end = start;
	while (end < len)
	{
		if (line[end] == '\\' && quotes == '\"'
			&& end + 1 < len)
			end += 2;
		else if (line[end] == quotes)
			break ;
		else
			end++;
	}
	return (end);
}

char	*get_temp_remove_quotes(char *line, int *i, char quotes)
{
	int		start;
	int		end;
	int		len;
	char	*result;

	len = ft_strlen(line);
	start = *i + 1;
	end = get_quote_end(line, start, len, quotes);
	result = malloc((end - start + 1) * sizeof(char));
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

int	check_closing_quote(char *line, int *i, char quote)
{
	while (line[*i])
	{
		if (line[*i] == '\\' && quote == '\"')
			return (write_stderr("The character is not supported"), -1);
		if (line[*i] == quote)
			return (1);
		(*i)++;
	}
	return (0);
}

int	check_quotes_loop(char *line, int *i, int j)
{
	while (line[*i])
	{
		if (line[*i] == line[j])
		{
			(*i)++;
			return (1);
		}
		(*i)++;
	}
	return (0);
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
		if (line[i] == '\'' || line[i] == '\"')
		{
			j = i;
			i++;
			end_q = check_quotes_loop(line, &i, j);
		}
		else
			i++;
	}
	if (end_q == 0)
	{
		write_stderr("The quotes are not closed");
		return (1);
	}
	return (0);
}

// echo "Hello world and the fucking goverment! '$USER' $PATH "$USER"\ $$$"