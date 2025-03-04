/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_redir_cmd_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:53:33 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/04 17:31:03 by tfarkas          ###   ########.fr       */
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
	st->childs = NULL;
	st->fd = 1;
	st->pidcount = 0;
}

void	reset_fds(t_store *st)
{
	if (dup2(st->save_stdin, STDIN_FILENO) < 0)
	{
		perror("Failed to reset STDIN_FILENO");
		exit(EXIT_FAILURE);
	}
	if (dup2(st->save_stdout, STDOUT_FILENO) < 0)
	{
		perror("Failed to reset STDIN_FILENO");
		exit(EXIT_FAILURE);
	}
	close(st->save_stdin);
	close(st->save_stdout);
	if (st->fd > 2)
		close(st->fd);
}

int	cmd_fds_reset(char **cmd, t_store *st)
{
	int	status;

	status = choose_cmd(*cmd, st);
	reset_fds(st);
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

int	wait_child(t_store *st)
{
	int	i;
	int	status;

	i = 0;
	while (i < st->pidcount)
	{
		waitpid(st->childs[i], &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
		else
			return (1);
		i++;
	}
	free(st->childs);
	st->childs = NULL;
	st->pidcount = 0;
	return (status);
}
