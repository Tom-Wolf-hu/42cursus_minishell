/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 13:54:02 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/03 10:54:32 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	hcp_prev_fd(t_pipe_data *data, int i)
{
	if (data->prev_fd != -1 && !has_heredoc(data->commands[i]))
	{
		dup2(data->prev_fd, STDIN_FILENO);
		close(data->prev_fd);
	}
}

void	handle_child_process(t_pipe_data data, int i, int *status,
	char ***myenvp)
{
	char				**cmd_args;
	char				*clean_cmd;
	char				*path;
	struct s_saved_std	std;

	hcp_prev_fd(&data, i);
	save_and_redirect(&std, data.commands[i], status, *myenvp);
	if (!pipe_cmd_exist(&data, &clean_cmd, i))
		return (exit(0));
	cmd_args = ft_split(clean_cmd, ' ');
	if (i < data.num_commands - 1)
		dup2(data.pipefd[1], STDOUT_FILENO);
	close_pipefd(&data);
	if (is_builtin(data.commands[i]))
		return (execute_builtin(data.commands[i], 1, status, myenvp),
			exit(*status));
	cmd_args = process_command_args(cmd_args);
	path = get_command_path(cmd_args[0], *myenvp);
	if (!path)
		return (write_stderr("Command not found"), exit(127));
	return (close_saved_std(&std), execve(path, cmd_args, *myenvp),
		perror("execve"), exit(EXIT_FAILURE));
}

void	handle_parent_process(struct s_pipe_data *data, int i, pid_t pid,
		int *last_pid)
{
	if (data->prev_fd != -1)
		close(data->prev_fd);
	if (i < data->num_commands - 1)
		close(data->pipefd[1]);
	data->prev_fd = data->pipefd[0];
	if (i == data->num_commands - 1)
		*last_pid = pid;
}

void	hanlde_pid(struct s_pipe_data *data, int *status, int *last_pid,
		char ***myenvp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		handle_child_process(*data, data->i, status, myenvp);
	else
		handle_parent_process(data, data->i, pid, last_pid);
}

void	execute_pipe_commands(char *cmd, int *status, char ***myenvp)
{
	int					last_pid;
	struct s_pipe_data	data;

	if (!init_data(&data, cmd, status))
		return ;
	while (data.i < data.num_commands)
	{
		if (data.i < data.num_commands - 1 && pipe(data.pipefd) == -1)
			return (perror("pipe"), exit(EXIT_FAILURE));
		hanlde_pid(&data, status, &last_pid, myenvp);
		data.i++;
	}
	wait_for_last_pid(last_pid, status);
	free_arr(data.commands);
}

// void	execute_pipe_commands(char *cmd, int *status, char ***myenvp)
// {
// 	int					last_pid;
// 	char				*temp;
// 	struct s_pipe_data	data;

// 	data.cmd = cmd;
// 	temp = NULL;
// 	ft_bzero(&data, sizeof(data));
// 	data.commands = get_commands(cmd, temp);
// 	if (temp)
// 		free(temp);
// 	if (!data.commands)
// 		return ;
// 	if (!check_data_cmd(&data))
// 	{
// 		*status = 258;
// 		return ;
// 	}
// 	data.num_commands = ft_arrlen(data.commands);
// 	data.prev_fd = -1;
// 	data.i = 0;
// 	while (data.i < data.num_commands)
// 	{
// 		if (data.i < data.num_commands - 1 && pipe(data.pipefd) == -1)
// 			return (perror("pipe"), exit(EXIT_FAILURE));
// 		hanlde_pid(&data, status, &last_pid, myenvp);
// 		data.i++;
// 	}
// 	wait_for_last_pid(last_pid, status);
// 	free_arr(data.commands);
// }
