/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/03/26 19:17:22 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile int	g_status = 0;

void	ft_error(char *error, int exit_status)
{
	perror(error);
	exit(exit_status);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_status = 130;
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
	
	(void)fd;
	commands = ft_split(cmd, '|');
	if (!commands)
		return;

	while (commands[num_commands])
		num_commands++;

	prev_fd = 0;
	for (i = 0; i < num_commands; i++)
	{
		pipe(pipefd);

		pid = fork();
		if (pid == -1)
			return (perror("fork"), exit(EXIT_FAILURE));
		if (pid == 0)
		{
			if (i > 0)
				dup2(prev_fd, STDIN_FILENO);
			if (i < num_commands - 1)
				dup2(pipefd[1], STDOUT_FILENO);
			if (is_builtin(commands[i]))
			{
				execute_builtin(commands[i], 1, status);
				exit(*status);
			}
			close(pipefd[0]);
			close(pipefd[1]);
			// printf("command before spliting by space: %s\n", commands[i]);
			cmd_args = ft_split(commands[i], ' ');
			int j = 0;
			while (cmd_args[j])
			{
				// printf("hello\n");
				// printf("command after spliting by space: %s\n", cmd_args[j]);
				if (ft_strchr(cmd_args[j], '\'') != NULL)
				{
					// printf("got a quote\n");
					remove_chars(&cmd_args[j], '\'');
				}
				j++;
			}
			char *path = get_command_path(cmd_args[0]);
			if (!path)
			{
				printf("%s: Command not found\n", cmd);
				exit(127);
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
		if (WIFEXITED(*status))
			*status = WEXITSTATUS(*status);
	}
	for (i = 0; i < num_commands; i++)
		free(commands[i]);
	free(commands);
}

void	execute_command_single(char *cmd, int *status, t_line *sline)
{
	char **cmd_arr;
	char *path;
	int pid;
	int wstatus;

	redir_line(sline);
	if (is_builtin(cmd))
		return (execute_builtin(cmd, 1, status));
	cmd_arr = ft_split(cmd, ' ');
	if (!cmd_arr || !*cmd_arr)
		exit(EXIT_FAILURE);
	path = get_command_path(cmd_arr[0]);
	if (!path)
	{
		*status = 127;
		printf("%s: Command not found\n", cmd);// ---------------------------------------------------
		return ;
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
	free_arr(cmd_arr);
	// set_back_stds();
}

void	run_ex(char **line, int *status)
{
	t_line sline;


	init_line(&sline);
	if (is_empty(*line))
		return ;
	add_history(*line);
	check_quastion_sign(line, ft_itoa(*status));
	bridge_var(line);
	store_lines(*line, &sline);

	// if (!ft_strchr(*line, '|'))
	// 	return (execute_command_single(*line, status));
	// execute_pipe_commands(*line, 1, status);
	
	// write(1, "passed1\n", 8);
	printf("The content of cmd_l: %s\n", sline.cmd_l);
	
	// write(1, "passed2\n", 8);
	printf("This is the content of redir_l[0]: %s\n", sline.redir_l[0]);
	printf("This is the content of redir_l[1]: %s\n", sline.redir_l[1]);
	if (!ft_strchr(sline.cmd_l, '|'))
		return (execute_command_single(sline.cmd_l, status, &sline));
	execute_pipe_commands(sline.cmd_l, 1, status);

	// write(1, "passed3\n", 8);


	int count = 0;

	count = count_rps(sline.redir_l[0]);
	printf("This is the number parts of redir_l[0]: %d\n", count);
	count = count_rps(sline.redir_l[1]);
	printf("This is the number parts of redir_l[1]: %d\n", count);
	
	// redir_ch(&sline, sline.redir_l[0]);
	// print_arr(sline.redir_parts);

	// redir_ch(&sline, sline.redir_l[1]);
	// print_arr(sline.redir_parts);
	// set_back_stds();
	free_line(&sline);
}

void	dis_echo_insc(struct termios *old_term)
{
	struct termios	new_term;

	// write(1, "passed1\n", 8);
	if (!isatty(STDIN_FILENO))
	{
		tcgetattr(STDIN_FILENO, old_term);
		// write(1, "passed2\n", 8);
		new_term = *old_term;
		new_term.c_lflag &= ~ECHO;
		tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
		// write(1, "passed3\n", 8);
	}

}

void	restore_echo(struct termios *old_term)
{
	if (isatty(STDIN_FILENO))
		return ;
	// tcsetattr(STDIN_FILENO, TCSANOW, old_term);
	old_term->c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}

int main(void)
{
	char			*line;
	static int		status = 0;
	struct termios	old_term;

	dis_echo_insc(&old_term);
	setup_signal_handlers();
	disable_ctrl_c_output(&status);
	while (1)
	{
		// check_tty();
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			line = readline("");
		// printf("This is the line: %s\n", line);
		if (g_status == 130)
		{
			status = g_status;
			g_status = 0;
		}
		if (!line || ft_strcmp(line, "exit") == 0 || ft_strncmp(line, "exit ", 5) == 0)
		{
			handle_exit(line, &status);
			restore_echo(&old_term);
			break ;
		}
		else if (ft_strcmp(line, "clear") == 0 || ft_strncmp(line, "clear ", 6) == 0)
			rl_clear_history();
		else
			run_ex(&line, &status);
		free(line);
		set_back_stds();
	}
	restore_echo(&old_term);
	rl_clear_history();
	// free(line);
}
// status after ctrl + c

// не рабатает:
// cd			+
// export		+
// env | wc -l	+
// полное отсутсвие redirections
// ps aux | grep bash | awk '{print $2}'


/* 
план redirections:
нужно иметь еще одну с самого начала, которая будет чисто убирать пробелы между командой, знаком и файло (cat > file ==> cat>file)
также должна быть ф-я, которая будет перехватывать эти "команды" и обабатывать их, потому что, если эта херня попадется в execve, то 
обработка будет происходить неправильно

как это сделать:
в ф-ях execute_pipe_commands() и execute_command_single() дабавляем в цикл вызов ф-ии проверки reedirectons и если он попадается, то мы его полностью обабатываем,
вместе с командой (то есть ф-я должна будет сама обрабатывать cat > file) и затем просто либо сдвигать i из цикла, ли бо переписывать саму строку и возвращать уже чтото
новое, либо лучше она это сделает через **

# Введите Ctrl+C внутри minishell (появляется лишний > в приглашении)
sleep 5


*/