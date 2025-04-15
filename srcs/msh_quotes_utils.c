/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 18:03:50 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/15 09:20:15 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	rqfw_copy(char *line, char *result, int i)
{
	int	j;

	j = 0;
	while (line[i] && line[i] != '\'' && line[i] != '\"')
	{
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
			return (1);
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
	return (0);
}

char	*remove_quotes_first_word(char *line)
{
	int		i;
	char	*result;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (line[i] != '\'' && line[i] != '\"')
		return (ft_strdup(line));
	result = malloc(ft_strlen(line) - i - 2 + 1);
	if (!result)
		return (NULL);
	i++;
	if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
		return (free(result), NULL);
	if (rqfw_copy(line, result, i) == 1)
		return (free(result), NULL);
	return (result);
}

int	ch_quotes_part(char *line, int *i, int *end_q)
{
	int	j;

	if (line[*i] == '\'' || line[*i] == '\"')
	{
		j = *i;
		(*i)++;
		*end_q = 0;
		while (line[*i])
		{
			if (line[*i] == '\\' && line[j] == '\"')
				return (write_stderr("The character is not supported"), 1);
			if (line[*i] == line[j])
			{
				*end_q = 1;
				break ;
			}
			(*i)++;
		}
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
		if (line[i] == ';' || line[i] == '\\')
			return (write_stderr("The character is not supported"), 1);
		if (ch_quotes_part(line, &i, &end_q) == 1)
			return (1);
		i++;
	}
	if (end_q == 0)
	{
		write_stderr("The quotes are not closed");
		return (1);
	}
	return (0);
}

// int	check_quotes(char *line)
// {
// 	int	i;
// 	int	j;
// 	int	end_q;

// 	i = 0;
// 	end_q = 1;
// 	while (line[i])
// 	{
// 		if (line[i] == ';' || line[i] == '\\')
// 			return (write_stderr("The character is not supported"), 1);
// 		if (line[i] == '\'' || line[i] == '\"')
// 		{
// 			j = i;
// 			i++;
// 			end_q = 0;
// 			while (line[i])
// 			{
// 				if (line[i] == '\\' && line[j] == '\"')
// 					return (write_stderr("The character is not supported"), 1);
// 				if (line[i] == line[j])
// 				{
// 					end_q = 1;
// 					break ;
// 				}
// 				i++;
// 			}
// 		}
// 		i++;
// 	}
// 	if (end_q == 0)
// 	{
// 		write_stderr("The quotes are not closed");
// 		return (1);
// 	}
// 	return (0);
// }

// char	*remove_quotes_first_word(char *line)
// {
// 	int		i;
// 	int		j;
// 	char	*result;

// 	i = 0;
// 	j = 0;
// 	if (!line)
// 		return (NULL);
// 	while (line[i] && ft_isspace(line[i]))
// 		i++;
// 	if (line[i] != '\'' && line[i] != '\"')
// 		return (ft_strdup(line));
// 	result = malloc(ft_strlen(line) - 2 + 1);
// 	if (!result)
// 		return (NULL);
// 	i++;
// 	if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
// 		return (NULL);
// 	while (line[i] && line[i] != '\'' && line[i] != '\"')
// 	{
// 		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
// 			return (free(result), NULL);
// 		result[j] = line[i];
// 		j++;
// 		i++;
// 	}
// 	if (line[i] == '\'' || line[i] == '\"')
// 		i++;
// 	while (line[i])
// 	{
// 		result[j] = line[i];
// 		j++;
// 		i++;
// 	}
// 	result[j] = '\0';
// 	return (result);
// }

/*
	check if we still need the following three function.
	I think we don't need anymore.
*/

char	*remove_quotes_commands(char *line)
{
	char	**arr;
	char	*clean_result;
	char	*clean_str;
	int		i;

	i = 0;
	arr = ft_split(line, '|');
	while (arr[i])
	{
		clean_str = remove_redirects(arr[i]);
		if (!clean_str)
			return (free_arr(arr), NULL);
		if (!check_command_quotes(clean_str))
			return (free(clean_str), free_arr(arr), NULL);
		clean_str = remove_quotes_first_word(clean_str);
		if (!clean_str)
			return (NULL);
		free(arr[i]);
		arr[i] = clean_str;
		i++;
	}
	clean_result = ft_join(arr);
	return (free_arr(arr), clean_result);
}

int	check_command_quotes(char *line)
{
	int	in_quote;
	int	i;

	in_quote = 0;
	i = 0;
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

char	*ft_join(char **arr)
{
	int		i;
	char	*result;
	char	*temp;

	i = 0;
	if (!*arr)
		return (NULL);
	result = ft_strdup(arr[i]);
	if (!result)
		return (NULL);
	while (arr[i + 1])
	{
		temp = ft_strjoin(result, " ");
		if (!temp)
			return (free(result), NULL);
		free(result);
		result = temp;
		temp = ft_strjoin(result, arr[i + 1]);
		if (!temp)
			return (free(result), NULL);
		free(result);
		result = temp;
		i++;
	}
	return (result);
}
