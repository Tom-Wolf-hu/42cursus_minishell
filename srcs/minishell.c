/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/04/04 14:40:46 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_status = 0;

void	ft_error(char *error, int exit_status)
{
	perror(error);
	exit(exit_status);
}

void	sig_handler(int sig)
{
	int		status;
	pid_t	pid;

	if (sig == SIGINT)
	{
		g_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		pid = waitpid(-1, &status, WNOHANG);
		if (pid == 0)
			return ;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		// rl_done = 1;
	}
	if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

int	check_line(char *line, int i)
{
	int	len;

	if (!line)
		return (1);
	len = ft_strlen(line);
	while (i < len)
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*remove_first_spaces(char *line)
{
	char	*new_line;
	int		i;

	i = 0;
	new_line = NULL;
	if (is_empty(line))
		return (new_line);
	while (line[i] && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'
		|| line[i] == '\v' || line[i] == '\f' || line[i] == '\r'))
	{
		i++;
	}
	new_line = ft_strdup(line + i);
	return (new_line);
}

void	disable_ctrl_c_output(int *status)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	*status = 130;
}

void	setup_signal_handlers(void)
{
	struct sigaction sa;

	sa.sa_handler = sig_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

int	is_nummeric(char *line)
{
	int i = 0;

	if (!line)
		return (1);
	while (line[i])
	{
		if (line[i] < '0' || line[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	finish_write_cmd_path(char **buffer, char *path, char *cmd)
{
	int i;
	int j;
	int len;

	i = 0;
	j = 0;
	if (!path || !cmd)
	{
		return (0);
	}
	len = ft_strlen(path) + ft_strlen(cmd);
	*buffer = malloc(len + 2);
	if (!*buffer)
		return (0);
	while (path[i])
	{
		(*buffer)[i] = path[i];
		i++;
	}
	(*buffer)[i] = '/';
	i++;
	while (cmd[j])
	{
		(*buffer)[i] = cmd[j];
		i++;
		j++;
	}
	(*buffer)[i] = '\0';
	return (1);
}

char *get_command_path(char *cmd)
{
	char **path_arr;
	char *buffer;
	char *path;
	int i;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	buffer = NULL;
	path = getenv("PATH");
	if (!path)
		return (NULL);
	path_arr = ft_split(path, ':');
	if (!path_arr || !*path_arr)
		return (NULL);
	i = 0;
	while (path_arr[i])
	{
		if (!finish_write_cmd_path(&buffer, path_arr[i], cmd))
		{
			free_arr(path_arr);
			return (NULL);
		}
		if (access(buffer, X_OK) == 0)
		{
			free_arr(path_arr);
			return (buffer);
		}
		free(buffer);
		i++;
	}
	free_arr(path_arr);
    return NULL;
}

void execute_pipe_commands(char *cmd, int fd, int *status)
{
	char **commands; // {"ls -l", "wc -l"}
	char **cmd_args; // {"ls", "-l"}
	int num_commands = 0;
	int i;
	int pipefd[2];
	pid_t pid;
	int prev_fd;
	struct s_saved_std std;
	char *clean_cmd;
	int wstatus;
	// printf("[execute_pipe_commands] starting\n");
	commands = ft_split(cmd, '|');
	if (!commands)
		return;
	while (commands[num_commands])
		num_commands++;
	prev_fd = -1;
	i = 0;
	while (i < num_commands)
	{
		if (i < num_commands - 1 && pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		// if (prev_fd != -1)//////////
		// 	close(prev_fd);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), exit(EXIT_FAILURE));
		if (pid == 0)
		{
			std.saved_stdin = dup(STDIN_FILENO);
			std.saved_stdout = dup(STDOUT_FILENO);
			handle_redirection(commands[i], status);
			if (prev_fd != 0)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);//
			}
			if (i < num_commands - 1)
				dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			// if (prev_fd != -1)///////////
				// close(prev_fd);
			if (is_builtin(commands[i]))
			{
				execute_builtin(commands[i], 1, status);
				exit(*status);
			}
			clean_cmd = remove_redirects(commands[i]);
			cmd_args = ft_split(clean_cmd, ' ');
			// printf("clean_cmd: %s\n", clean_cmd);
			// std.saved_stdin = dup(STDIN_FILENO);
			// std.saved_stdout = dup(STDOUT_FILENO);
			// handle_redirection(commands[i], status);
			int j = 0;
			while (cmd_args[j])
			{
				if (ft_strchr(cmd_args[j], '\'') != NULL)
				{
					remove_chars(&cmd_args[j], '\'');
					// break ;
				}
				if (ft_strchr(cmd_args[j], '\"') != NULL)
				{
					remove_chars(&cmd_args[j], '\"');
					// break ;
				}
				j++;
			}
			char *path = get_command_path(cmd_args[0]);
			if (!path)
			{
				printf("%s: Command not found\n", cmd);
				exit(127);
			}
			close(std.saved_stdin);
			close(std.saved_stdout);
			execve(path, cmd_args, NULL);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		else
		{
			waitpid(pid, &wstatus, 0);
			if (WIFEXITED(wstatus))
			{
				*status = WEXITSTATUS(wstatus);
				if (*status == 127)
				{
					printf("Error: Command not found\n");
					break;
				}
			}
			else if (WIFSIGNALED(wstatus))
			{
				*status = 128 + WTERMSIG(wstatus);
			}
			if (prev_fd != -1)
                close(prev_fd); // Закрываем только если он уже был открыт
            close(pipefd[1]); // Закрываем запись в пайп
            prev_fd = pipefd[0];
		}
		i++;
	}
	// waitpid(pid, status, 0);
	if (prev_fd != -1)
		close(prev_fd);
	free_arr(commands);
}

void	execute_command_single(char *cmd, int *status)
{
	char **cmd_arr;
	char *path;
	int pid;
	int wstatus;
	struct s_saved_std std;
	char *clean_cmd;
	char *line;

	// printf("[execute_command_single] starting\n");
	if (is_builtin(cmd))
	{
		execute_builtin(cmd, 1, status);
		return ;
	}
	clean_cmd = remove_redirects(cmd);
	if (clean_cmd && is_empty(clean_cmd))
	{
		free(clean_cmd);
		clean_cmd = NULL;
	}
	if (!clean_cmd)
	{
		std.saved_stdout = dup(STDOUT_FILENO);
		std.saved_stdin = dup(STDIN_FILENO);
		handle_redirection(cmd, status);
		if (*status == 1)
		{
			dup2(std.saved_stdin, STDIN_FILENO);
			dup2(std.saved_stdout, STDOUT_FILENO);
			close(std.saved_stdin);
			close(std.saved_stdout);
			return ;
		}
		line = get_next_line(STDIN_FILENO);
		while (line)
		{
			write(STDOUT_FILENO, line, ft_strlen(line));
			free(line);
			line = get_next_line(STDIN_FILENO);
		}
		dup2(std.saved_stdin, STDIN_FILENO);
		dup2(std.saved_stdout, STDOUT_FILENO);
		close(std.saved_stdin);
		close(std.saved_stdout);
		// write(2, "2passed1\n", 9);
		return ;
	}
	cmd_arr = ft_split(clean_cmd, ' ');
	if (!cmd_arr || !*cmd_arr)
		exit(EXIT_FAILURE);
	path = get_command_path(cmd_arr[0]);
	if (!path)
	{
		*status = 127;
		printf("%s: Command not found\n", clean_cmd);
		return ;
	}
	std.saved_stdin = dup(STDIN_FILENO);
	std.saved_stdout = dup(STDOUT_FILENO);
	handle_redirection(cmd, status);
	if (*status == 1)
	{
		dup2(std.saved_stdin, STDIN_FILENO);
		dup2(std.saved_stdout, STDOUT_FILENO);
		close(std.saved_stdin);
		close(std.saved_stdout);
	}
	pid = fork();
	if (pid == 0)
	{
		if (!path)
			exit(EXIT_FAILURE);
		execve(path, cmd_arr, NULL);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &wstatus, 0);
		if (WIFEXITED(wstatus))
		{
			*status = WEXITSTATUS(wstatus);
		}
		else if (WIFSIGNALED(wstatus))
		{
			*status = 128 + WTERMSIG(wstatus);
		}
	}
	else
	{
		perror("fork");
		*status = 1;
	}
	dup2(std.saved_stdin, STDIN_FILENO);
	dup2(std.saved_stdout, STDOUT_FILENO);
	close(std.saved_stdin);
	close(std.saved_stdout);
	free_arr(cmd_arr);
}

void	run_ex(char **line, int *status)
{
	char **arr;
	char *clean_cmd;
	char *new_line;

	if (is_empty(*line))
		return ;
	add_history(*line);
	if (check_quotes(*line) == 1)
		return ;
	check_quastion_sign(line, ft_itoa(*status));
	bridge_var(line);
	
	clean_cmd = remove_quotes_commands(*line);
	if (!clean_cmd)
	{
		printf("wrong command\n");
		return ;
	}

	if (!ft_strchr(clean_cmd, '|'))
		return (execute_command_single(clean_cmd, status));
	execute_pipe_commands(clean_cmd, 1, status);
}

int	main(void)
{
	char			*line;
	static int		status = 0;

	disable_ctrl_c_output(&status);
	setup_signal_handlers();
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
		{
			line = get_next_line(STDIN_FILENO);
		}
		if (g_status == 130)
		{
			status = g_status;
			g_status = 0;
		}
		// if (!line || ft_strcmp(line, "exit") == 0 || ft_strncmp(line, "exit ", 5) == 0)
		// {
		// 	handle_exit(line, &status);
		// 	break ;
		// }
		if (!line)
		{
			// handle_exit(line, &status);
			break ;
		}
		else if (ft_strcmp(line, "clear") == 0 || ft_strncmp(line, "clear ", 6) == 0)
			rl_clear_history();
		else
			run_ex(&line, &status);
		free(line);
	}
	rl_clear_history();
	free(line);
}

// status after ctrl + c
// не рабатает:
// cd			+
// export		+
// env | wc -l	+
// полное отсутсвие redirections
// ps aux | grep bash | awk '{print $2}'
// Введите Ctrl+C внутри minishell (появляется лишний > в приглашении)
// sleep 5

// <file wc -l не работает (потому что все удаляется)

/* 
# Введите Ctrl+C внутри minishell (появляется лишний > в приглашении)
sleep 5

ctrl + \ after some stuff should do nothing
если писать cat и затем нажимать ctrl+c появляется лишний > 

echo "hello       wolrd"
The command not found
*/