/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:15:08 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/26 15:08:54 by omalovic         ###   ########.fr       */
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

/*Previously it was the following function line

void	choose_redirection(t_tokentype e_red, char *name_d)
{
	if (!name_d)
	{
		ft_putendl_fd("File name or delimiter does \
			ot exist in choose_op.", STDERR_FILENO);
		return ;
	}
	if (e_red == REDINPUT)
		red_in(name_d);
	else if (e_red == REDOUTPUT)
		red_del(name_d);
	else if (e_red == APPENDREDOUTPUT)
		red_out_append(name_d);
	else if (e_red == REDDELIMETER)
		red_del(name_d);
}
*/

void	choose_redirection(t_tokentype e_red, char *name_d)
{
	if (!name_d)
	{
		ft_putendl_fd("File name or delimiter does \
			ot exist in choose_op.", STDERR_FILENO);
		return ;
	}
	if (e_red == REDINPUT)
		red_in(name_d);
	else if (e_red == REDOUTPUT)
		red_del(name_d);
	else if (e_red == APPENDREDOUTPUT)
		red_out_append(name_d);
	else if (e_red == REDDELIMETER)
		red_del(name_d);
}
