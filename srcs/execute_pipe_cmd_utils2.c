/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_cmd_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:14:08 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/03 10:53:13 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_data(t_pipe_data *data, char *cmd, int *status)
{
	char	*temp;

	data->cmd = cmd;
	temp = NULL;
	ft_bzero(data, sizeof(data));
	data->commands = get_commands(cmd, temp);
	if (temp)
		free(temp);
	if (!data->commands)
		return (0);
	if (!check_data_cmd(data))
	{
		*status = 258;
		return (0);
	}
	data->num_commands = ft_arrlen(data->commands);
	data->prev_fd = -1;
	data->i = 0;
	return (1);
}

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
