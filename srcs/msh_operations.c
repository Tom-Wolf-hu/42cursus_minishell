/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:15:08 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/24 19:01:02 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	choose_redirection(t_tokentype e_red, char *name_d, t_store *st)
{
	if (!name_d)
	{
		ft_putendl_fd("Filename or delimiter does \
			not exist in choose_op.", STDERR_FILENO);
		return ;
	}
	if (e_red == REDINPUT)
		red_in(name_d);
	else if (e_red == REDOUTPUT)
		st->fd_exout = red_out(name_d);
	else if (e_red == APPENDREDOUTPUT)
		st->fd_exout = red_out_append(name_d);
	else if (e_red == REDDELIMETER)
		red_del(name_d);
}

void	set_red(char *redir, t_tokentype e_red, int *i)
{
	if (redir[*i] == '<' && redir[*i + 1] != '<')
	{
		e_red = REDINPUT;
		(*i) += 1;
	}
	else if (redir[*i] == '<' && redir[*i + 1] == '<')
	{
		e_red = REDDELIMETER;
		(*i) += 2;
	}
	else if (redir[*i] == '>' && redir[*i + 1] != '>')
	{
		e_red = REDOUTPUT;
		(*i) += 1;
	}
	else if (redir[*i] == '>' && redir[*i + 1] == '>')
	{
		e_red = APPENDREDOUTPUT;
		(*i) += 2;
	}
	else
	{
		perror("Problem with redirection characters");
		exit(EXIT_FAILURE);
	}
}

int		count_rps(char *redir)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	if (!redir)
		return (count);
	write(1, "passed1\n", 8);
	while (redir[i])
	{
		while (redir[i] && ft_isspace(redir[i]))
			i++;
		if (redir[i] == '<' || redir[i] == '>')
			count++;
		while (redir[i] && (redir[i] == '<' || redir[i] == '>'))
			i++;
		while (redir[i] && redir[i] != '<' && redir[i] == '>')
			i++;
	}
	write(1, "passed2\n", 8);
	return (count);
}

void	redir_ch(char *redir, t_tokentype e_red)
{
	int		i;
	int		start;
	char	*name_d;

	i = 0;
	skip_whites(redir, &i);
	set_red(redir, e_red, &i);
	skip_whites(redir, &i);
	start = i;
	while (redir[i] && redir[i] != '<'
		&& redir[i] != '>' && !ft_isspace(redir[i]))
	{
		i++;
	}
	if (i == start)
		return ;
	name_d = ft_substr(redir, start, i - start + 1);
	if (!name_d)
	{
		perror("Failed to allocate memory name_d");
		exit(EXIT_FAILURE);
	}
	free(name_d);
}

// void	redir_line(t_line *sline)
// {
// 	t_tokentype	e_red;

// 	if (sline->cmd_l == 0 && sline->redir_l[0])
// 	{
// 		while (sline->redir_l[0][i])
// 		{
			
// 		}
// 	}
// 	if (sline->pipecount > 0 && sline->cmd_l == sline->pipecount
// 		&& sline->redir_l[1])
// 	{
		
// 	}
// }

// int	count_deilemeter(char *line, char delimeter)
// {
// 	int	count;

// 	count = 0;
// 	while (line[count] && (line[count] == delimeter || ft_isspace(line[count])))
// 		count++;
// 	if (count > 2)
// 	{
// 		ft_putendl_fd("Syntax error", STDERR_FILENO);
// 		exit(EXIT_FAILURE);
// 	}
// 	return (count);
// }

// char	*allocate_word(char *line, int len)
// {
// 	char	*word;
// 	int		i;

// 	i = 0;
// 	if (len < 1)
// 		return (NULL);
// 	word = (char *)malloc((len + 1) * sizeof(char));
// 	if (!word)
// 	{
// 		perror("Failed to allocate memory for word");
// 		return (NULL);
// 	}
// 	while (i < len)
// 	{
// 		word[i] = line[i];
// 		i++;
// 	}
// 	word[i] = '\0';
// 	return (word);
// }

// char	*save_w(char *line, char delimeter)
// {
// 	int		start;
// 	int		end;
// 	char	*word;

// 	start = 0;
// 	while (line[start] && line[start] != delimeter)
// 		start++;
// 	start = start + count_deilemeter(line + start, delimeter);
// 	end = start;
// 	while (line[end] && (!ft_isoperator(line[end]) || !ft_isspace(line[end])))
// 		end++;
// 	if (start == end)
// 		return (NULL);
// 	word = allocate_word(line + start, end - start);
// 	while (line[end] && !ft_isoperator(line[end]))
// 		end++;
// 	return (word);
// }

// void	check_redirect(char *line)
// {
// 	int 	i;
// 	char	*filename;	

// 	i = 0;
// 	while (line[i])
// 	{
// 		filename = save_w(line, '>', &i);
// 		if (filename)
			
// 	}
// }