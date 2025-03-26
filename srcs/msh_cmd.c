/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:26:24 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/26 15:38:05 by omalovic         ###   ########.fr       */
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
		close(st->save_stdin);
		close(st->save_stdout);
		// close(st->fd);
		fds_state();
		if (execve(cmdp, cmdarg, environ) == -1)
		{
			perror("Failed to execute the command.\n");
			exit(127);
		}
	}
	else if (pid > 0)
		save_chpid(pid, st);
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
	else if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "env ") == 0)
		return (1);
	else if (ft_strncmp(cmd, "export ", 7) == 0 ||
		ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset ", 6) == 0 || ft_strcmp(cmd, "unset") == 0)
		return (1);
	else
		return (0);
}

void	handle_heredoc(const char *delimiter)
{
	int pipe_fd[2];
	char *line = NULL;
	size_t len = 0;

	pipe(pipe_fd); // Создаём пайп

	// Читаем строки, пока не встретится delimiter
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		if (getline(&line, &len, stdin) == -1)
			break;
		// Если строка совпадает с delimiter — выходим
		if (strncmp(line, delimiter, strlen(delimiter)) == 0 && line[strlen(delimiter)] == '\n')
			break;
		write(pipe_fd[1], line, strlen(line)); // Пишем в пайп
	}
	free(line);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO); // Перенаправляем stdin на пайп
	close(pipe_fd[0]);
}

char *get_filename(char *cmd)
{
	char *filename;
	int j;
	int i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '>' || cmd[i] == '<')
			break ;
		i++;
	}
	if (cmd[i] == '>' || cmd[i] == '<')
		i++;
	while (cmd[i] && (cmd[i] == 32 || cmd[i] == '\t' || cmd[i] == '\n'))
		i++;
	j = 0;
	filename = malloc(ft_strlen(cmd) - i + 1);
	if (!filename)
		return (NULL);
	while (cmd[i])
	{
		filename[j] = cmd[i];
		i++;
		j++;
	}
	filename[j] = '\0';
	return (filename);
}

void	execute_builtin(char *cmd, int fd, int *status)
{
	int saved_stdin;
	int saved_stdout;
	char *filename;
	int file_fd;
	char *redir;

	if (!cmd)
		return ;
	filename = get_filename(cmd);
	printf("filename: %s, len: %d\n", filename, ft_strlen(filename));
	*status = 1;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	// file_fd = -1;
	
	// обработка here-document (<<)
	// if ((redir = strstr(cmd, "<<")))
	// {
	// 	*redir = '\0';
	// 	redir += 2;
	// 	while (*redir == ' ')
	// 		redir++;
	// 	handle_heredoc(redir);
	// }
	// // Добавление в файл (>>)
	// else if ((redir == strstr(cmd, ">>")))
	// {
	// 	*redir = '\0';
	// 	redir += 2;
	// 	while (*redir == ' ')
	// 		redir++;
	// 	file_fd = open(redir, O_WRONLY | O_CREAT | O_APPEND, 0644);
	// 	if (file_fd == -1)
	// 		return (perror("open"), *status = 1, (void)0);
	// 	dup2(file_fd, STDOUT_FILENO);
	// 	close(file_fd);
	// }
	// Запись в файл (>)
	if ((redir = strchr(cmd, '>')))
	{
		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file_fd == -1)
		{
			perror("open");
			*status = 1;
			return;
		}
		dup2(file_fd, STDOUT_FILENO);
		close(file_fd);
	}
	// // Обрабатываем '<' (чтение из файла)
	// else if ((redir = strchr(cmd, '<')))
	// {
	// 	*redir = '\0';
	// 	redir++;
	// 	while (*redir == ' ') redir++;

	// 	file_fd = open(redir, O_RDONLY);
	// 	if (file_fd == -1)
	// 	{
	// 		perror("open");
	// 		*status = 1;
	// 		return;
	// 	}
	// 	dup2(file_fd, STDIN_FILENO);
	// 	close(file_fd);
	// }

	if (ft_strcmp(cmd, "pwd") == 0 || ft_strncmp(cmd, "pwd ", 4) == 0)
		*status = ft_getcwd(cmd, fd);
	else if (ft_strncmp(cmd, "cd ", 3) == 0 || ft_strcmp(cmd, "cd") == 0)
		*status = handle_cd(cmd);
	else if (ft_strncmp(cmd, "echo ", 5) == 0 || ft_strcmp(cmd, "echo") == 0)
		*status = handle_echo(cmd, fd);
	else if (ft_strcmp(cmd, "env") == 0 || ft_strcmp(cmd, "env ") == 0)
		*status = print_env(fd);
	else if (ft_strncmp(cmd, "export ", 7) == 0 || ft_strcmp(cmd, "export") == 0)
		*status = handle_export(cmd, fd);
	else if (ft_strncmp(cmd, "unset ", 6) == 0 || ft_strcmp(cmd, "unset") == 0)
		*status = handle_unset(cmd, fd);
	
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
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
		execute_builtin(cmd, st->fd_exout, status);
		exit(*status);
	}
	else if (pid > 0)
		save_chpid(pid, st);
	return (1);
}

int	choose_cmd(char *line, t_store *st)
{
	int		status;
	char	*new_line;

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

