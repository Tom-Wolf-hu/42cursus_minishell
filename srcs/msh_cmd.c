/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:26:24 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/17 19:06:05 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*shearch_cmd(char *cmd)
{
	char	*result;
	char	*env_varval;
	char	**env_path;
	int		i;

	i = 0;
	env_varval = env_variable_val("PATH");
	// printf("'%s'\n", env_varval);
	if (!env_varval)
		return (free(env_varval), NULL);
	env_path = dev_strarr(env_varval, ':');
	if (!env_path)
		return (NULL);
	while (env_path[i] != NULL)
	{
		result = cmd_acces(env_path[i], cmd);
		if (result)
			break ;
		i++;
	}
	free_arr(env_path);
	// return (free(env_varval), result);
	return (result);
}

char	*cmd_path(char *cmd)
{
	char	*path;
	char	**arg;

	path = NULL;
	if (cmd[0] == '/' || ft_strncmp("./", cmd, 2) == 0)
		path = ft_strdup(cmd);
	else
	{
		arg = dev_strarr(cmd, ' ');
		if (arg && arg[0])
			path = shearch_cmd(arg[0]);
	}
	if (!path || access(path, X_OK) < 0)
		return (NULL);
	return (path);
}

int	execute_cmd(char *cmd, t_store *st)
{
	pid_t		pid;
	char		*cmdp;
	char		**cmdarg;
	extern char	**environ;
	// char	*args[] = {cmd, NULL};

	cmdp = cmd_path(cmd);
	// printf("This is the cmdp: \t%s\n", cmdp);
	cmdarg = dev_strarr(cmd, ' ');
	// printf("This is the cmdarg: \t%s\n", cmdarg[0]);
	// print_arr(cmdarg);
	if (!cmdp || !cmdarg)
	{
		ft_putendl_fd("The commandpath does not exists.", 2);
		return (127);
	}
	pid = fork();
	if (pid == 0)
	{
		// check_tty();
		chproc_fd(st);
		// fds_state();
		if (execve(cmdp, cmdarg, environ) == -1)
		{
			perror("Failed to execute the command.\n");
			exit(127);
		}
	}
	else if (pid > 0)
	{
		save_chpid(pid, st);
		parent_fd(st);
	}
	else
	{
		perror("Failed to create fork.\n");
		return (1);
	}
	free(cmdp);
	free_arr(cmdarg);
	return (0);
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "pwd") == 0 || ft_strncmp(cmd, "pwd ", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd ", 3) == 0 || ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strncmp(cmd, "echo ", 5) == 0 || ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
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
	if (ft_strcmp(cmd, "pwd") == 0 || ft_strncmp(cmd, "pwd ", 4) == 0)
		*status = ft_getcwd(cmd, fd);
	else if (ft_strncmp(cmd, "cd ", 3) == 0 || ft_strcmp(cmd, "cd") == 0)
		*status = handle_cd(cmd);
	else if (ft_strncmp(cmd, "echo ", 5) == 0 || ft_strcmp(cmd, "echo") == 0)
		*status = handle_echo(cmd, fd);
	else if (ft_strcmp(cmd, "env") == 0)
		*status = print_env(fd);
	else if (ft_strncmp(cmd, "export ", 7) == 0 || ft_strcmp(cmd, "export") == 0)
		*status = handle_export(cmd, fd);
	else if (ft_strncmp(cmd, "unset ", 6) == 0 || ft_strcmp(cmd, "unset") == 0)
		*status = handle_unset(cmd, fd);
}

int builtin_check(char *cmd, t_store *st, int *status)
{
	pid_t	pid;

	if (is_builtin(cmd) == 0)
		return (0);
	if (st->pipecount == 0)
	{
		execute_builtin(cmd, st->fd_exout, status);
		return (1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Failed to create fork.");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		chproc_fd(st);
		// execute_builtin(cmd, st->fd_exout, status);
		execute_builtin(cmd, STDOUT_FILENO, status);
		exit(*status);
	}
	save_chpid(pid, st);
	parent_fd(st);
	return (1);
}

int	choose_cmd(char *line, t_store *st)
{
	int		status;
	char	*new_line;

	// fds_state();
	new_line = remove_first_spaces(line);
	status = 0;
	if (is_empty(new_line))
	{
		return (free(new_line), status);
	}
	if (builtin_check(new_line, st, &status) == 1)
	{
		return (free(new_line), status);
	}
	else
	{
		status = execute_cmd(new_line, st);
		// printf("exit status execute_cmd: %i\n", status);
		// status = wait_child(st);
		// printf("exit status wait_child: %i\n", status);
	}
	return (free(new_line), status);
}

