/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:46:55 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/02 18:08:43 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	redir_part(char *cmd, int *i)
{
	while (cmd[*i] && (cmd[*i] == '<' || cmd[*i] == '>' || ft_isspace(cmd[*i])))
		(*i)++;
	while (cmd[*i] && !ft_isspace(cmd[*i]))
		(*i)++;
}

char	*before_red(char *cmd, int *i)
{
	int		start;
	int		j;
	char	*bef_red;

	start = *i;
	j = 0;
	while (cmd[*i] && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	if (start == *i)
		return (NULL);
	bef_red = malloc((*i - start + 1));
	if (!bef_red)
	{
		perror("Failed to allocate memory for bef_red");
		exit(EXIT_FAILURE);
	}
	while (start < *i)
	{
		bef_red[j] = cmd[start];
		start++;
		j++;
	}
	bef_red[j] = '\0';
	return (bef_red);
}

void	quotes_redir_len(char *cmd, int *i, char *quote)
{
	while (cmd[*i])
	{
		if ((cmd[*i] == '\'' || cmd[*i] == '\"'))
		{
			if (!(*quote))
				*quote = cmd[*i];
			else if (*quote == cmd[*i])
				*quote = 0;
		}
		if ((cmd[*i] == '<' || cmd[*i] == '>') && !(*quote))
			break ;
		(*i)++;
	}
}

void	remove_reidr_temp(char *cmd, int i, int start, char **clean_cmd)
{
	char	*temp;

	temp = malloc(i - start + 1);
	if (!temp)
	{
		perror("Failed to allocate memory in remove_redirects");
		exit(EXIT_FAILURE);
	}
	ft_memcpy(temp, cmd + start, i - start);
	temp[i - start] = '\0';
	join_part(clean_cmd, temp);
}

char	*remove_redirects(char *cmd)
{
	char	*clean_cmd;
	int		i;
	int		start;
	char	quote;

	i = 0;
	clean_cmd = NULL;
	quote = 0;
	while (cmd[i])
	{
		start = i;
		quotes_redir_len(cmd, &i, &quote);
		if (i > start)
			remove_reidr_temp(cmd, i, start, &clean_cmd);
		if ((cmd[i] == '<' || cmd[i] == '>') && !quote)
			redir_part(cmd, &i);
	}
	return (clean_cmd);
}
