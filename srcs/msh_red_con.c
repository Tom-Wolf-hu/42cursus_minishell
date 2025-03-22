/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_red_con.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:22:13 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/22 18:27:30 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_line(t_line *sline)
{
	sline->cmd_l = NULL;
	sline->redir_l = (char **)ft_calloc(3, sizeof(char *));
	if (!sline->redir_l)
	{
		perror("Failed to allocate memory for redir_l");
		free(sline->cmd_l);
		exit(EXIT_FAILURE);
	}
	sline->tokarr[0] = -1;
	sline->tokarr[1] = -1;
	sline->pipecount = 0;
}

void	free_line(t_line *sline)
{
	free(sline->cmd_l);
	if (sline->redir_l)
		free_arr(sline->redir_l);
}

char	*ft_crjoin(char	*s1, char *s2)
{
	char	*newstr;
	int		lens1;
	int		lens2;
	int		i;
	int		j;

	if (s1 == NULL)
		s1 = ft_strdup("");
	if (s2 == NULL)
		s2 = ft_strdup("");
	if (!s1 || !s2)
		return (free(s1), free(s2), NULL);
	lens1 = ft_strlen(s1);
	lens2 = ft_strlen(s2);
	i = 0;
	j = 0;
	if (lens1 + lens2 == 0)
		return (free(s1), free(s2), NULL);
	newstr = (char *)ft_calloc(lens1 + lens2 +1, sizeof(char));
	if (!newstr)
		return (free(s1), free(s2), NULL);
	while (i < lens1 + lens2)
	{
		if (i < lens1 && s1[i] != '\0')
			newstr[i] = s1[i];
		else if (lens2 > 0)
		{
			newstr[i] = s2[j];
			j++;
		}
		i++;
	}
	newstr[i] = '\0';
	return (free(s1), free(s2), newstr);
}

void	store_redir(char *line, int *i, t_line *sline)
{
	int		len;
	int		start;
	char	*save_redir;

	len = 0;
	start = *i;
	while (line[*i] && (line[*i] == '>' || line[*i] == '<'))
		(*i)++;
	if (*i - start > 2)
	{
		perror("Syntax error: to many redirections");
		return ;
	}
	while (line[*i] && ft_isspace(line[*i]))
		(*i)++;
	while (line[*i] && (line[*i] != '>' || line[*i] != '<'
		|| !ft_isspace(line[*i]) || line[*i] != '|'))
	{
		(*i)++;
	}
	save_redir = ft_substr(line, start, *i - start + 1);
	if (!save_redir)
	{
		perror("Failed to use ft_substr to save redirection part");
		exit(EXIT_FAILURE);
	}
	sline->redir_l[0] = ft_crjoin(sline->redir_l[0], save_redir);
	if (!sline->redir_l[0])
	{
		perror("Failed to join save redirection part");
		exit(EXIT_FAILURE);
	}
}

void	store_cmd(char *line, int *i, t_line *sline)
{
	int	len;
	int start;
	char *save_cmdp;
	
	len = 0;
	start = *i;
	while (line[*i] && (line[*i] != '>' && line[*i] != '<'))
	{
		(*i)++;
		len++;
	}
	if (start == *i)
		return ;
	save_cmdp = ft_substr(line, start, len);
	if (!save_cmdp)
	{
		perror("Failed to use ft_substr to save command part");
		exit(EXIT_FAILURE);
	}
	// printf("This is save_cmdp: %s\n", save_cmdp);
	sline->cmd_l = ft_crjoin(sline->cmd_l, save_cmdp);
	if (!sline->cmd_l)
	{
		perror("Failed to join command part");
		exit(EXIT_FAILURE);
	}
}

void	store_lines(char *line, t_line *sline)
{
	int	i;

	i = 0;
	if (!line)
		return ;
	while (line[i])
	{
		if (skip_whites(line, &i))
		{
			// printf("This is line[%d]: %c\n", i, line[i]);
			if (line[i] == '<' || line[i] == '>')
			{
				// write(1, "passed\n", 7);
				store_redir(line, &i, sline);
			}
			else
				store_cmd(line, &i, sline);
		}
		// if (line[i] != '\0')
		// 	i++;
	}
}