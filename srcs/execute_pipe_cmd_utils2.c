/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_cmd_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:14:08 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/02 14:20:15 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_data_cmd(t_pipe_data *data)
{
	int	i;

	i = 0;
	while (data->commands[i])
	{
		if (is_empty(data->commands[i]))
		{
			write_stderr("syntax error near unexpected token `|'");
			return (0);
		}
		i++;
	}
	return (1);
}

void	wait_for_last_pid(pid_t last_pid, int *status)
{
	pid_t	wpid;
	int		wstatus;

	wpid = waitpid(-1, &wstatus, 0);
	while (wpid > 0)
	{
		if (wpid == last_pid)
		{
			if (WIFEXITED(wstatus))
				*status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				*status = 128 + WTERMSIG(wstatus);
		}
		wpid = waitpid(-1, &wstatus, 0);
	}
}

void	close_saved_std(struct s_saved_std *std)
{
	if (std->saved_stdin != -1)
		close(std->saved_stdin);
	if (std->saved_stdout != -1)
		close(std->saved_stdout);
}
