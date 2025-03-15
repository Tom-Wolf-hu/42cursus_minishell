/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_redir_cmd_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:53:33 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/15 19:24:38 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_store(t_store	*st)
{
	st->save_stdin = dup(STDIN_FILENO);
	if (st->save_stdin < 0)
	{
		perror("Failed to duplicate standard input");
		return ;
	}
	st->save_stdout = dup(STDOUT_FILENO);
	if (st->save_stdout < 0)
	{
		perror("Failed to duplicate standard output");
		return ;
	}
	st->pidcount = 0;
	st->childs = NULL;
	st->fd_exin = 0;
	st->fd_exout = 1;
	st->pidcount = 0;
	st->cmd_num = 0;
}

void	reset_fds(t_store *st)
{
	// fds_state();
	// check_tty();
	// printf("%i\n", st->save_stdin);
	// printf("%i\n", STDIN_FILENO);
	if (dup2(st->save_stdin, STDIN_FILENO) < 0)
	{
		perror("Failed to reset STDIN_FILENO");
		exit(EXIT_FAILURE);
	}
	if (dup2(st->save_stdout, STDOUT_FILENO) < 0)
	{
		perror("Failed to reset STDOUT_FILENO");
		exit(EXIT_FAILURE);
	}
	close(st->save_stdin);
	close(st->save_stdout);
	close(st->fd_readl);
	if (st->fd_exin > 2)
		close(st->fd_exin);
	if (st->fd_exout > 2)
		close(st->fd_exout);
	// fds_state();
}

int	cmd_fds_reset(char **cmd, t_store *st)
{
	int	status;

	status = choose_cmd(*cmd, st);
	// reset_fds(st);
	free(*cmd);
	return (status);
}

void	save_chpid(pid_t pid, t_store *st)
{
	int	pidlen;

	pidlen = 0;
	if (st->pidcount == 0 || st->pidcount > pidlen)
	{
		pidlen += 5;
		st->childs = ft_realloc(st->childs, st->pidcount * sizeof(pid_t),
				pidlen * sizeof(pid_t));
		if (!st->childs)
		{
			perror("Failed to allocate memory for pid array");
			return ;
		}
	}
	st->childs[st->pidcount] = pid;
	st->pidcount++;
}

int	wait_child(t_store *st, int status)
{
	int	i;
	int	temp_status;

	i = 0;
	temp_status = 0;
	while (i < st->pidcount)
	{
		waitpid(st->childs[i], &temp_status, 0);
		if (WIFEXITED(temp_status))
			status = WEXITSTATUS(temp_status);
		else if (WIFSIGNALED(temp_status))
			status = 128 + WTERMSIG(temp_status);
		i++;
	}
	free(st->childs);
	st->childs = NULL;
	st->pidcount = 0;
	return (status);
}
