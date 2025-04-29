/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_cmd_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:33:52 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/29 13:34:23 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	**process_command_args(char **cmd_args, char *cmd)
{
	int		j;
	char	*clean_cmd2;

	j = 0;
	if (!cmd_args)
		return (perror("malloc"), exit(1), NULL);
	while (cmd_args[j])
	{
		clean_cmd2 = remove_quotes(cmd_args[j]);
		if (!clean_cmd2)
		{
			printf("%s: Command not found\n", cmd_args[j]);
			exit(127);
		}
		free(cmd_args[j]);
		cmd_args[j] = clean_cmd2;
		j++;
	}
	return (cmd_args);
}

void	save_and_redirect(struct s_saved_std *std, char *command,
			int *status, char **envp)
{
	std->saved_stdin = dup(STDIN_FILENO);
	std->saved_stdout = dup(STDOUT_FILENO);
	handle_redirection(command, status, envp);
}

void	close_saved_std(struct s_saved_std *std)
{
	if (std->saved_stdin != -1)
		close(std->saved_stdin);
	if (std->saved_stdout != -1)
		close(std->saved_stdout);
}

int	has_heredoc(char *cmd)
{
	return (ft_strnstr(cmd, "<<", ft_strlen(cmd)) != NULL);
}
