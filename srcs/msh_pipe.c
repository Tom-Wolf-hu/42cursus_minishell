/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:19:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/26 18:19:57 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_dup(int pipefd[2], int which, char *beforep, char *afterp)
{
	if (which == 1)
	{
		if (dup2(pipefd[0], STDOUT_FILENO) < 0)
			return ;
		close(pipefd[0]);
		choose_cmd(beforep);
	}
	else if (which == 2)
	{
		if (dup2(pipefd[1], STDIN_FILENO) < 0)
			return ;
		close(pipefd[1]);
		choose_cmd(afterp);
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