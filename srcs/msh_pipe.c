/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:19:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/04 19:26:05 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
The pipe part still didn't tested. I wrote the t_store st 
just that case we can compile the program. Later I need to go deep how we wnat to solve it.
*/
void	pipe_dup(int pipefd[2], int which, char *beforep, char *afterp)
{
	t_store	*st;

	st = NULL;
	if (which == 1)
	{
		if (dup2(pipefd[0], STDOUT_FILENO) < 0)
			return ;
		close(pipefd[0]);
		choose_cmd(beforep, st);
	}
	else if (which == 2)
	{
		if (dup2(pipefd[1], STDIN_FILENO) < 0)
			return ;
		close(pipefd[1]);
		choose_cmd(afterp, st);
	}
}

void	ft_pipe(char *beforep, char *afterp)
{
	int	pipefd[2];

	if (pipe(pipefd) < 0)
	{
		perror("Pipe creation failed.\n");
		return ;
	}
	if (beforep)
	{
		close(pipefd[1]);
		pipe_dup(pipefd, 1, beforep, afterp);
	}
	if (afterp)
	{
		close(pipefd[0]);
		pipe_dup(pipefd, 2, beforep, afterp);
	}
	close(pipefd[0]);
	close(pipefd[1]);
}

void temp_redline(char *line)
{
	int	fd_readl;
	int	i;

	i = 0;
	fd_readl = open(".temp_readline", O_WRONLY | O_CREAT, 0644);
	if (fd_readl < 0)
	{
		perror("Failed open fd_readl filedescriptor.");
		return ;
	}
	while (line[i])
	{
		if (line[i] == '|')
			write(fd_readl, "\n", 1);
		else
			write(fd_readl, line[i], 1);
		i++;
	}
	close(fd_readl);
}
