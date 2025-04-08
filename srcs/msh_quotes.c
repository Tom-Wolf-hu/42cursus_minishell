/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:25:56 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/08 18:05:32 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_command_quotes(char *line)
{
    int in_quote = 0;
    int i = 0;

    while (line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;
    if ((line[i] == '\'' || line[i] == '\"') && line[i + 1] == line[i])
        return 0;
    while (line[i]) 
    {
        if ((line[i] == '\'' || line[i] == '\"') && !in_quote) 
        {
            in_quote = 1;
        }
        else if ((line[i] == '\'' || line[i] == '\"') && in_quote) 
        {
            in_quote = 0;
        }
        if (isspace(line[i]) && in_quote)
            return 0;
        i++;
    }
    if (in_quote)
        return 0;
    return 1;
}

char *remove_quotes_first_word(char *line)
{
	int i = 0;
	int j = 0;
	char *result;

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

char	*remove_quotes_commands(char *line)
{
	char **arr;
	char *clean_result;
	char *clean_str;
	int i = 0;

	arr = ft_split(line, '|');
	while (arr[i])
	{
		clean_str = remove_redirects(arr[i]);
		if (!clean_str)
		{
			free_arr(arr);
			return (NULL);
		}
		if (!check_command_quotes(clean_str))
		{
			free(clean_str);
			free_arr(arr);
			return (NULL);
		}
		clean_str = remove_quotes_first_word(clean_str);
		if (!clean_str)
			return (NULL);
		// printf("%s\n", clean_str);
		free(arr[i]);
		arr[i] = clean_str;
		i++;
	}
	clean_result = ft_join(arr);
	return (free_arr(arr), clean_result);
}

char *ft_join(char **arr)
{
	int i = 0;
	char *result;
	char *temp;

	if (!*arr)
		return (NULL);
	result = strdup(arr[i]);
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
		{
			free(result);
			return (NULL);
		}
		free(result);
		result = temp;
		i++;
	}
	return (result);
}

char *get_temp_remove_quotes(char *line, int *i, char quotes)
{
    int len;
    int start;
    int end;
	char *result;

	len = strlen(line);
	start = *i + 1;
	end = start;
    while (end < len)
	{
        if (line[end] == '\\' && quotes == '\"' && end + 1 < len && (quotes == '\"' || quotes == '\''))
            end += 2;
        else if (line[end] == quotes)
            break;
		else
            end++;
    }
    result = (char *)malloc((end - start + 1) * sizeof(char));
    if (!result)
		return NULL;
    memcpy(result, line + start, end - start);
    result[end - start] = '\0';
    if (end < len && line[end] == quotes)
		*i = end + 1;
	else
		*i = end;
    return result;
}

char *remove_quotes(char *line)
{
    int i;
	int len;
    char *result;
	int pos;
	char *tmp;

	i = 0;
	len = strlen(line);
	result = (char *)malloc(len + 1);
    if (!result)
		return NULL;
    pos = 0;
    while (i < len)
	{
        if (line[i] == '\'' || line[i] == '\"')
		{
            tmp = get_temp_remove_quotes(line, &i, line[i]);
            if (tmp)
			{
                strcpy(result + pos, tmp);
                pos += strlen(tmp);
                free(tmp);
            }
        }
		else
            result[pos++] = line[i++];
    }
    result[pos] = '\0';
    return result;
}

void	write_stderr(char *str)
{
	int	strlen;

	if (!str)
		str = "Error occured.\n";
	strlen = ft_strlen(str);
	write(2, str, strlen);
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
			end_q = 0;
			while (line[i])
			{
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
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	return (0);
}
