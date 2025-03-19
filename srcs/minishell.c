/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/03/19 17:15:25 by alex             ###   ########.fr       */
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

int handle_redirection(char *cmd) {
    char *in_file = NULL, *out_file = NULL;
    int append = 0, heredoc = 0;
    char *token = strtok(cmd, " ");  // Разбираем строку на части
    char *clean_cmd[100];  // Очищенная команда без редиректов
    int clean_index = 0;
    
    while (token) {
        if (strcmp(token, "<") == 0) {  // Входной редирект
            token = strtok(NULL, " ");
            if (!token) return -1;
            in_file = token;
        }
        else if (strcmp(token, ">") == 0) {  // Выходной редирект (перезапись)
            token = strtok(NULL, " ");
            if (!token) return -1;
            out_file = token;
            append = 0;
        }
        else if (strcmp(token, ">>") == 0) {  // Выходной редирект (добавление)
            token = strtok(NULL, " ");
            if (!token) return -1;
            out_file = token;
            append = 1;
        }
        else if (strcmp(token, "<<") == 0) {  // Heredoc (<< delimiter)
            token = strtok(NULL, " ");
            if (!token) return -1;
            heredoc = 1;
            in_file = token;
        }
        else {
            clean_cmd[clean_index++] = token;  // Обычная команда
        }
        token = strtok(NULL, " ");
    }
    clean_cmd[clean_index] = NULL;

    // Обрабатываем входной редирект (< file)
    if (in_file && !heredoc) {
        int fd = open(in_file, O_RDONLY);
        if (fd == -1) {
            perror("open input file");
            return -1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    // Обрабатываем выходной редирект (> file или >> file)
    if (out_file) {
        int fd = open(out_file, O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
        if (fd == -1) {
            perror("open output file");
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    // Обрабатываем heredoc (<< delimiter) через get_next_line()
    if (heredoc) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            return -1;
        }
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return -1;
        }
        if (pid == 0) {  // Дочерний процесс записывает в pipe
            close(pipefd[0]);
            char *line;
            while (1) {
                write(1, "heredoc> ", 9);
                line = get_next_line(0);
                if (!line) break;
                if (strncmp(line, in_file, strlen(in_file)) == 0 && line[strlen(in_file)] == '\n') {
                    free(line);
                    break;
                }
                write(pipefd[1], line, strlen(line));
                free(line);
            }
            close(pipefd[1]);
            exit(0);
        } else {  // Родительский процесс читает из pipe
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            wait(NULL);
        }
    }

    // Возвращаем новую команду без редиректов
    strcpy(cmd, "");
    for (int i = 0; i < clean_index; i++) {
        strcat(cmd, clean_cmd[i]);
        if (i < clean_index - 1)
            strcat(cmd, " ");
    }
    
    return 0;
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
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (i > 0)
				dup2(prev_fd, STDIN_FILENO);
			if (i < num_commands - 1)
				dup2(pipefd[1], STDOUT_FILENO);
			if (is_builtin(commands[i]))
			{
				// if (i < num_commands - 1)
					// dup2(pipefd[1], STDOUT_FILENO);
				// printf("commands[i]: %s\n", commands[i]);
				execute_builtin(commands[i], fd, status);
				exit(*status);
			}
			close(pipefd[0]);
			close(pipefd[1]);
			////////////////////////////////////////////////////////////////////////
			// handle_redirection(commands[i]);
			if (handle_redirection(commands[i]) == -1) {
				exit(EXIT_FAILURE);
			}
			////////////////////////////////////////////////////////////////////////
			cmd_args = ft_split(commands[i], ' ');
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

void	execute_command_single(char *cmd, int *status)
{
	char **cmd_arr;
	char *path;
	int pid;
	int wstatus;

	if (is_builtin(cmd))
		return (execute_builtin(cmd, 1, status));
	//////////////////////////////////////////////
	// wstatus = handle_redirection(cmd);
	// if (wstatus)
		// return ;
	// if (wstatus == -1)
		// exit(EXIT_FAILURE);
	if (handle_redirection(cmd) == -1) {
		*status = 1;
		return;
	}
	//////////////////////////////////////////////
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
		// printf("cmd has worked!\n");
	}
	else
	{
		perror("fork");
		*status = 1;
	}
	free_arr(cmd_arr);
}

void	run_ex(char **line, int *status)
{
	if (is_empty(*line))
		return ;
	add_history(*line);
	check_quastion_sign(line, ft_itoa(*status));
	bridge_var(line);
	if (!ft_strchr(*line, '|'))
		return (execute_command_single(*line, status));
	execute_pipe_commands(*line, 1, status);
	// if (is_builtin(*line))
		// execute_builtin(*line, 1, status);
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
		if (g_status == 130)
		{
			status = g_status;
			g_status = 0;
		}
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

// не рабатает:
// cd
// export
// env | wc -l
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
*/