/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:25:56 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/14 21:11:30 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	gtrq_search_quotes(char *line, int *end, char quotes)
{
	int	len;

	len = ft_strlen(line);
	while (*end < len)
	{
		if (line[*end] == '\\' && quotes == '\"' && (*end) + 1 < len
			&& (quotes == '\"' || quotes == '\''))
			(*end) += 2;
		else if (line[*end] == quotes)
			break ;
		else
			(*end)++;
	}
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
	gtrq_search_quotes(line, &end, quotes);
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

void	rq_copy(char *line, char *result, int len)
{
	int		i;
	int		pos;
	char	*tmp;

	i = 0;
	pos = 0;
	while (i < len)
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			tmp = get_temp_remove_quotes(line, &i, line[i]);
			if (tmp)
			{
				ft_memcpy(result + pos, tmp, ft_strlen(tmp));
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
	int		len;
	char	*result;
	char	*tmp;

	len = ft_strlen(line);
	result = (char *)malloc(len + 1);
	if (!result)
	{
		perror("Malloc failed in remove_quotes");
		exit(EXIT_FAILURE);
	}
	rq_copy(line, result, len);
	return (result);
}



// char	*remove_quotes_commands(char *line)
// {
// 	char	**arr;
// 	char	*clean_result;
// 	char	*clean_str;
// 	int		i;

// 	i = 0;
// 	arr = ft_split(line, '|');
// 	while (arr[i])
// 	{
// 		clean_str = remove_redirects(arr[i]);
// 		if (!clean_str)
// 		{
// 			free_arr(arr);
// 			return (NULL);
// 		}
// 		if (!check_command_quotes(clean_str))
// 		{
// 			free(clean_str);
// 			free_arr(arr);
// 			return (NULL);
// 		}
// 		clean_str = remove_quotes_first_word(clean_str);
// 		if (!clean_str)
// 			return (NULL);
// 		// printf("%s\n", clean_str);
// 		free(arr[i]);
// 		arr[i] = clean_str;
// 		i++;
// 	}
// 	clean_result = ft_join(arr);
// 	return (free_arr(arr), clean_result);
// }

// char	*get_temp_remove_quotes(char *line, int *i, char quotes)
// {
// 	int		len;
// 	int		start;
// 	int		end;
// 	char	*result;

// 	len = strlen(line);
// 	start = *i + 1;
// 	end = start;
// 	while (end < len)
// 	{
// 		if (line[end] == '\\' && quotes == '\"' && end + 1 < len
// 			&& (quotes == '\"' || quotes == '\''))
// 			end += 2;
// 		else if (line[end] == quotes)
// 			break ;
// 		else
// 			end++;
// 	}
// 	result = (char *)malloc((end - start + 1) * sizeof(char));
// 	if (!result)
// 		return (NULL);
// 	memcpy(result, line + start, end - start);
// 	result[end - start] = '\0';
// 	if (end < len && line[end] == quotes)
// 		*i = end + 1;
// 	else
// 		*i = end;
// 	return (result);
// }

// char	*remove_quotes(char *line)
// {
// 	int		i;
// 	int		len;
// 	char	*result;
// 	int		pos;
// 	char	*tmp;

// 	i = 0;
// 	len = strlen(line);
// 	result = (char *)malloc(len + 1);
// 	if (!result)
// 		return (NULL);
// 	pos = 0;
// 	while (i < len)
// 	{
// 		if (line[i] == '\'' || line[i] == '\"')
// 		{
// 			tmp = get_temp_remove_quotes(line, &i, line[i]);
// 			if (tmp)
// 			{
// 				strcpy(result + pos, tmp);
// 				pos += strlen(tmp);
// 				free(tmp);
// 			}
// 		}
// 		else
// 			result[pos++] = line[i++];
// 	}
// 	result[pos] = '\0';
// 	return (result);
// }
