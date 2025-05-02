/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_cmd_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:06:33 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/02 16:55:19 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_heredoc(char *cmd)
{
	return (ft_strnstr(cmd, "<<", ft_strlen(cmd)) != NULL);
}

void	close_pipefd(struct s_pipe_data *data)
{
	if (data->pipefd[0] != -1)
		close(data->pipefd[0]);
	if (data->pipefd[1] != -1)
		close(data->pipefd[1]);
}

int	pipe_cmd_exist(t_pipe_data *data, char **clean_cmd, int i)
{
	*clean_cmd = remove_redirects(data->commands[i]);
	if (is_empty(*clean_cmd))
		return (0);
	return (1);
}

int	save_and_redirect(struct s_saved_std *std, char *command,
	int *status, char **envp)
{
	std->saved_stdin = dup(STDIN_FILENO);
	std->saved_stdout = dup(STDOUT_FILENO);
	return (handle_redirection(command, status, envp));
}

char	**process_command_args(char **cmd_args)
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
