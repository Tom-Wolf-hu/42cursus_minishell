/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_redir_cmd_call.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 18:43:59 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/03 18:16:06 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_delimeter(char *line, char delimeter)
{
	int	count;

	count = 0;
	while (line[count] && line[count] == delimeter)
		count++;
	if (count > 2)
	{
		ft_putendl_fd("Syntax error", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	return (count);
}

void	redir_prep(char *filename, char delimeter, int count)
{
	t_tokentype e_red;

	if (delimeter == '<' && count == 1)
		e_red = REDINPUT;
	else if (delimeter == '<' && count == 2)
		e_red = REDDELIMETER;
	else if (delimeter == '>' && count == 1)
		e_red = REDOUTPUT;
	else if (delimeter == '>' && count == 2)
		e_red = APPENDREDOUTPUT;
	else
	{
		printf("Wrong delimeter: %c\n", delimeter);
		return ;
	}
	choose_redirection(e_red, filename);
}

void	redir_case(char *line, int *i)
{
	char	*filename;
	char	delimeter;
	int		count;
	int 	start;

	delimeter = line[*i];
	count = count_delimeter(line + (*i), delimeter);
	*i += count;
	filename = NULL;
	if (skip_whites(line, i))
	{
		start = *i;
		while (line[*i] && (!(ft_isoperator(line[*i]) || ft_isspace(line[*i]))))
			(*i)++;
		filename = ft_substr(line, start, (*i) - start);
		if (!filename)
		{
			perror("Error allocate memory for filename");
			return ;
		}
	}
	redir_prep(filename, delimeter, count);
	free(filename);
}

void	cmd_case(char *line, char *cmd, int *i, int *cmdlen)
{
	while (line[*i] && !ft_isoperator(line[*i]))
	{
		cmd[*cmdlen] = line[*i];
		(*cmdlen)++;
		(*i)++;
	}
	cmd[*cmdlen] = '\0';
}

int	redir_cmd_s(char *line)
{
	char	*cmd;
	int		i;
	int		cmdlen;
	t_store	st;

	i = 0;
	cmdlen = 0;
	init_store(&st);
	if (!line || ft_strlen(line) < 1)
		return (1);
	cmd = ft_calloc(ft_strlen(line), sizeof(char));
	if (!cmd)
	{
		perror("Error in memory allocation for the command.");
		return (1);
	}
	while (line[i])
	{
		if (skip_whites(line, &i) && ft_isoperator(line[i]))
			redir_case(line, &i);
		else
			cmd_case(line, cmd, &i, &cmdlen);
	}
	return (cmd_fds_reset(&cmd, &st));
}
