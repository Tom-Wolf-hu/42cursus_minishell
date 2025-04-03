/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:26:24 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/03 13:38:43 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0 || ft_strncmp(cmd, "exit ", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 3) == 0 || ft_strncmp(cmd, "pwd ", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd ", 3) == 0 || ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strncmp(cmd, "echo ", 5) == 0 || ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 3) == 0 || ft_strncmp(cmd, "env ", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export ", 7) == 0 ||
		ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset ", 6) == 0 || ft_strcmp(cmd, "unset") == 0)
		return (1);
	else
		return (0);
}

void	execute_builtin(char *cmd, int fd, int *status)
{
	struct s_saved_std std;
	char *clean_cmd;

	*status = 1;
	if (!cmd)
		return ;
	// printf("[execute_builtin] cmd: %s\n", cmd);
	clean_cmd = remove_redirects(cmd);
	// printf("[execute_builtin] clean_cmd: %s\n", clean_cmd);
	if (!clean_cmd)
		return ;
	// printf("clean_cmd: %s; len: %d\n", clean_cmd, ft_strlen(clean_cmd));
	// printf("filename: %s, len: %d\n", filename, ft_strlen(filename));
	std.saved_stdin = dup(STDIN_FILENO);
	std.saved_stdout = dup(STDOUT_FILENO);
	
	handle_redirection(cmd, status);
	// printf("[execute_buitin] clean_cmd: %s\n", clean_cmd);
	if (ft_strcmp(clean_cmd, "exit") == 0 || ft_strncmp(clean_cmd, "exit ", 5) == 0)
		handle_exit(cmd, status);
	if (ft_strcmp(clean_cmd, "pwd") == 0 || ft_strncmp(clean_cmd, "pwd ", 4) == 0)
		*status = ft_getcwd(clean_cmd, fd);
	else if (ft_strncmp(clean_cmd, "cd ", 3) == 0 || ft_strcmp(clean_cmd, "cd") == 0)
		*status = handle_cd(clean_cmd);
	else if (ft_strncmp(clean_cmd, "echo ", 5) == 0 || ft_strcmp(clean_cmd, "echo") == 0)
		*status = handle_echo(clean_cmd, fd);
	else if (ft_strcmp(clean_cmd, "env") == 0 || ft_strcmp(clean_cmd, "env ") == 0)
		*status = print_env(fd);
	else if (ft_strncmp(clean_cmd, "export ", 7) == 0 || ft_strcmp(clean_cmd, "export") == 0)
		*status = handle_export(clean_cmd, fd);
	else if (ft_strncmp(clean_cmd, "unset ", 6) == 0 || ft_strcmp(clean_cmd, "unset") == 0)
		*status = handle_unset(clean_cmd, fd);
	free(clean_cmd);
	dup2(std.saved_stdin, STDIN_FILENO);
	dup2(std.saved_stdout, STDOUT_FILENO);
	close(std.saved_stdin);
	close(std.saved_stdout);
}

// char	*cmd_path(char *cmd)
// {
// 	char	*path;
// 	char	**arg;

// 	path = NULL;
// 	if (cmd[0] == '/' || ft_strncmp("./", cmd, 2) == 0)
// 		path = ft_strdup(cmd);
// 	else
// 	{
// 		arg = dev_strarr(cmd, ' ');
// 		if (arg && arg[0])
// 			path = shearch_cmd(arg[0]);
// 	}
// 	if (!path || access(path, X_OK) < 0)
// 		return (NULL);
// 	return (path);
// }

// int	execute_cmd(char *cmd, t_store *st)
// {
// 	pid_t		pid;
// 	char		*cmdp;
// 	char		**cmdarg;
// 	extern char	**environ;
// 	// char	*args[] = {cmd, NULL};

// 	cmdp = cmd_path(cmd);
// 	// printf("This is the cmdp: \t%s\n", cmdp);
// 	cmdarg = dev_strarr(cmd, ' ');
// 	// printf("This is the cmdarg: \t%s\n", cmdarg[0]);
// 	// print_arr(cmdarg);
// 	if (!cmdp || !cmdarg)
// 	{
// 		ft_putendl_fd("The commandpath does not exists.", 2);
// 		return (127);
// 	}
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		// check_tty();
// 		close(st->save_stdin);
// 		close(st->save_stdout);
// 		// close(st->fd);
// 		fds_state();
// 		if (execve(cmdp, cmdarg, environ) == -1)
// 		{
// 			perror("Failed to execute the command.\n");
// 			exit(127);
// 		}
// 	}
// 	else if (pid > 0)
// 		save_chpid(pid, st);
// 	else
// 	{
// 		perror("Failed to create fork.\n");
// 		return (1);
// 	}
// 	free(cmdp);
// 	free_arr(cmdarg);
// 	return (0);
// }

// char	*shearch_cmd(char *cmd)
// {
// 	char	*result;
// 	char	*env_varval;
// 	char	**env_path;
// 	int		i;

// 	i = 0;
// 	env_varval = env_variable_val("PATH");
// 	// printf("'%s'\n", env_varval);
// 	if (!env_varval)
// 		return (free(env_varval), NULL);
// 	env_path = dev_strarr(env_varval, ':');
// 	if (!env_path)
// 		return (NULL);
// 	while (env_path[i] != NULL)
// 	{
// 		result = cmd_acces(env_path[i], cmd);
// 		if (result)
// 			break ;
// 		i++;
// 	}
// 	free_arr(env_path);
// 	// return (free(env_varval), result);
// 	return (result);
// }

// int builtin_check(char *cmd, t_store *st, int *status)
// {
// 	pid_t	pid;

// 	if (is_builtin(cmd) == 0)
// 		return (0);
// 	if (st->pipecount == 0)
// 	{
// 		execute_builtin(cmd, st->fd_exout, status);
// 		return (1);
// 	}
// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("Failed to create fork.");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 	{
// 		execute_builtin(cmd, st->fd_exout, status);
// 		exit(*status);
// 	}
// 	else if (pid > 0)
// 		save_chpid(pid, st);
// 	return (1);
// }

// int	choose_cmd(char *line, t_store *st)
// {
// 	int		status;
// 	char	*new_line;

// 	new_line = remove_first_spaces(line);
// 	status = 0;
// 	if (is_empty(new_line))
// 	{
// 		return (free(new_line), status);
// 	}
// 	if (builtin_check(new_line, st, &status) == 1)
// 	{
// 		return (free(new_line), status);
// 	}
// 	else
// 	{
// 		status = execute_cmd(new_line, st);
// 		// printf("exit status execute_cmd: %i\n", status);
// 		// status = wait_child(st);
// 		// printf("exit status wait_child: %i\n", status);
// 	}
// 	return (free(new_line), status);
// }
