/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_single.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:25:31 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/29 13:26:33 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_std(struct s_saved_std *std)
{
	dup2(std->saved_stdin, STDIN_FILENO);
	dup2(std->saved_stdout, STDOUT_FILENO);
	close_saved_std(std);
}

void	execute_forked_process(char *path, char **cmd_arr,
			int *status, char **myenvp)
{
	pid_t	pid;
	int		wstatus;

	if (!path)
		return ;
	pid = fork();
	if (pid == 0)
	{
		if (!path)
			exit(EXIT_FAILURE);
		execve(path, cmd_arr, myenvp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &wstatus, 0);
		if (WIFEXITED(wstatus))
			*status = WEXITSTATUS(wstatus);
		else if (WIFSIGNALED(wstatus))
			*status = 128 + WTERMSIG(wstatus);
	}
	else
		return (perror("fork"), *status = 1, exit(1));
}

char	**split_and_clean_args(char *clean_cmd, int *status)
{
	char	**cmd_arr;

	cmd_arr = ft_split(clean_cmd, ' ');
	free(clean_cmd);
	if (!cmd_arr || !*cmd_arr)
		exit(EXIT_FAILURE);
	if (clean_command_args(cmd_arr))
	{
		*status = 1;
		return (NULL);
	}
	return (cmd_arr);
}

char	*resolve_command_path(char *cmd_name, int *status, char **myenvp)
{
	char	*path;

	if (!cmd_name)
		return (NULL);
	path = get_command_path(cmd_name, myenvp);
	if (!path)
	{
		write_stderr("Command not found");
		*status = 127;
	}
	return (path);
}

void	execute_command_single(char *cmd, int *status, char ***myenvp)
{
	char				**cmd_arr;
	char				*path;
	struct s_saved_std	std;
	char				*clean_cmd;

	cmd_arr = NULL;
	if (is_builtin(cmd))
		return (execute_builtin(cmd, 1, status, myenvp));
	clean_cmd = remove_redirects(cmd);
	if (clean_cmd && is_empty(clean_cmd))
	{
		free(clean_cmd);
		clean_cmd = NULL;
	}
	if (!clean_cmd)
		return (handle_empty_command(cmd, status, *myenvp));
	cmd_arr = split_and_clean_args(clean_cmd, status);
	if (!cmd_arr)
		return ;
	path = resolve_command_path(cmd_arr[0], status, *myenvp);
	save_and_redirect(&std, cmd, status, *myenvp);
	if (*status == 1)
		return (restore_std(&std), free(path), free_arr(cmd_arr));
	execute_forked_process(path, cmd_arr, status, *myenvp);
	return (free(path), restore_std(&std), free_arr(cmd_arr));
}
