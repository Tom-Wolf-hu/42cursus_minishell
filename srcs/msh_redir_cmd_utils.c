/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_redir_cmd_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 13:53:33 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/03 18:57:02 by tfarkas          ###   ########.fr       */
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
}

int	cmd_fds_reset(char **cmd, t_store *st)
{
	int	status;

	status = choose_cmd(*cmd);
	reset_fds(st);
	free(*cmd);
	return (status);
}
