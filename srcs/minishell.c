/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/03/18 13:38:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error(char *error, int exit_status)
{
	perror(error);
	exit(exit_status);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_replace_line("", 1);
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

	commands = ft_split(cmd, '|');
	if (!commands)
		return;

	while (commands[num_commands])
		num_commands++;

	int prev_fd = 0;
	for (i = 0; i < num_commands; i++)
	{
		pipe(pipefd);

		pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (i > 0)
				dup2(prev_fd, STDIN_FILENO);
			if (i < num_commands - 1)
				dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			if (is_builtin(commands[i]))
			{
				if (i < num_commands - 1)
					dup2(pipefd[1], STDOUT_FILENO);
				execute_builtin(commands[i], fd, status);
				exit(*status);
			}
			cmd_args = ft_split(commands[i], ' ');
			char *path = get_command_path(cmd_args[0]);
			if (!path)
			{
				perror("Command not found");
				exit(EXIT_FAILURE);
			}
			execve(path, cmd_args, NULL);
			perror("execve");
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
		if (prev_fd != 0)
			close(prev_fd);
		prev_fd = pipefd[0];
		if (i == num_commands - 1)
			waitpid(pid, status, 0);
		else
			waitpid(pid, NULL, 0);
	}
	for (i = 0; i < num_commands; i++)
		free(commands[i]);
	free(commands);
}

void	run_ex(char **line, int *status)
{
	add_history(*line);
	check_quastion_sign(line, ft_itoa(*status));
	bridge_var(line);
	execute_pipe_commands(*line, 1, status);
}

int main(void)
{
	char			*line;
	static int		status = 0;

	// check_tty();
	disable_ctrl_c_output(&status);
	setup_signal_handlers();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, "exit") == 0 || ft_strncmp(line, "exit ", 5) == 0)
		{
			handle_exit(line, &status);
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