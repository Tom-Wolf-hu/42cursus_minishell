/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_redir_cmd_call.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 18:43:59 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/02 16:44:24 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_whites(char *line, int	*i)
{
	int	res;

	res = 0;
	while (line[*i] && ft_isspace(line[*i]))
		(*i)++;
	if (line[*i] != '\0')
		res = 1;
	return (res);
}

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
	if (delimeter == '<' && count == 1)
		printf("The redirectiom: redin.\n");
	else if (delimeter == '<' && count == 2)
		printf("The redirectiom: heredoc.\n");
	else if (delimeter == '>' && count == 1)
		printf("The redirectiom: redout.\n");
	else if (delimeter == '>' && count == 2)
		printf("The redirectiom: redoutappend.\n");
	printf("This is the redirection filename: \n%s\n\n", filename);
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

void	redir_cmd_s(char *line)
{
	char	*cmd;
	int		i;
	int		cmdlen;

	i = 0;
	cmdlen = 0;
	if (!line || ft_strlen(line) < 1)
		return ;
	cmd = ft_calloc(ft_strlen(line), sizeof(char));
	if (!cmd)
	{
		perror("Error in memory allocation for the command.");
		return ;
	}
	while (line[i])
	{
		if (skip_whites(line, &i) && ft_isoperator(line[i]))
			redir_case(line, &i);
		else
			cmd_case(line, cmd, &i, &cmdlen);
	}
	printf("This is the command: \n%s\n", cmd);
	free(cmd);
}

int main(void)
{
	char	*line;

	line = "hello>>  hi <<tamas < day hello hello";
	redir_cmd_s(line);
	return (0);
}