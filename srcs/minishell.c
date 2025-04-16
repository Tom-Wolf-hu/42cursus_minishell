/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/04/16 11:03:57 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile int	g_status = 0;

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
	}
	if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
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

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		perror("tcgetattr");
		*status = 1;
		return ;
	}
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("tcsetattr");
		*status = 1;
		return ;
	}
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_handler = sig_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

int	is_nummeric(char *line)
{
	int	i;

	i = 0;
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
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	if (!path || !cmd)
		return (0);
	len = ft_strlen(path) + ft_strlen(cmd);
	*buffer = malloc(len + 2);
	if (!*buffer)
		return (0);
	ft_strcpy(*buffer, path);
	ft_strcat(*buffer, "/");
	ft_strcat(*buffer, cmd);
	return (1);
}

char	*find_cmd_in_paths(char **path_arr, char *cmd)
{
	char	*buffer;
	int		i;

	i = 0;
	while (path_arr[i])
	{
		if (!finish_write_cmd_path(&buffer, path_arr[i], cmd))
			return (NULL);
		if (access(buffer, X_OK) == 0)
		{
			free_arr(path_arr);
			return (buffer);
		}
		free(buffer);
		i++;
	}
	return (NULL);
}

char	*get_command_path(char *cmd)
{
	char	**path_arr;
	char	*buffer;
	char	*path;
	int		i;

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
	return (find_cmd_in_paths(path_arr, cmd));
}

char	**get_commands(char *cmd, char *temp)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	if (cmd[i] == '|')
	{
		temp = malloc(ft_strlen(cmd) + 2);
		if (!temp)
			return (exit(1), NULL);
		temp[i] = ' ';
		i++;
		while (cmd[j])
		{
			temp[i] = cmd[j];
			j++;
			i++;
		}
		temp[i] = '\0';
		return (ft_split(temp, '|'));
	}
	return (ft_split(cmd, '|'));
}

void	wait_for_last_pid(pid_t last_pid, int *status)
{
	pid_t wpid;
	int wstatus;

	while ((wpid = waitpid(-1, &wstatus, 0)) > 0)
	{
		if (wpid == last_pid)
		{
			if (WIFEXITED(wstatus))
				*status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				*status = 128 + WTERMSIG(wstatus);
		}
	}
}

char	**process_command_args(char **cmd_args, char *cmd)
{
	int j = 0;
    char *clean_cmd2;

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
    return cmd_args;
}

void handle_child_process(t_pipe_data data, char *cmd, int i, int *status)
{
    char **cmd_args;
    char *clean_cmd;
    char *path;
    struct s_saved_std std;

    if (data.prev_fd != 0)
    {
        dup2(data.prev_fd, STDIN_FILENO);
        close(data.prev_fd);
    }
    if (i < data.num_commands - 1)
        dup2(data.pipefd[1], STDOUT_FILENO);
    close(data.pipefd[0]);
    close(data.pipefd[1]);
    if (is_builtin(data.commands[i]))
    {
        execute_builtin(data.commands[i], 1, status);
        exit(*status);
    }
    clean_cmd = remove_redirects(data.commands[i]);
    cmd_args = ft_split(clean_cmd, ' ');
    std.saved_stdin = dup(STDIN_FILENO);
    std.saved_stdout = dup(STDOUT_FILENO);
    handle_redirection(data.commands[i], status);
    cmd_args = process_command_args(cmd_args, cmd);
    path = get_command_path(cmd_args[0]);
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

void handle_parent_process(struct s_pipe_data *data, int i, pid_t pid, int *last_pid)
{
    if (data->prev_fd != -1)
        close(data->prev_fd);
    if (i < data->num_commands - 1)
        close(data->pipefd[1]);
    data->prev_fd = data->pipefd[0];
    if (i == data->num_commands - 1)
        *last_pid = pid;
}

void hanlde_pid(struct s_pipe_data *data, char *cmd, int *status, int *last_pid)
{
    pid_t pid;

	pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0)
        handle_child_process(*data, cmd, data->i, status);
    else
        handle_parent_process(data, data->i, pid, last_pid);
}

void	execute_pipe_commands(char *cmd, int *status)
{
	int					last_pid;
	char				*temp;
	struct s_saved_std	std;
	struct s_pipe_data	data = {0};

	temp = NULL;
	data.commands = get_commands(cmd, temp);
	if (temp)
		free(temp);
	if (!data.commands)
		return;
	data.num_commands = ft_arrlen(data.commands);
	data.prev_fd = -1;
	data.i = 0;
	while (data.i < data.num_commands)
	{
		if (data.i < data.num_commands - 1 && pipe(data.pipefd) == -1)
			return (perror("pipe"), exit(EXIT_FAILURE));
		hanlde_pid(&data, cmd, status, &last_pid);
		data.i++;
	}
	wait_for_last_pid(last_pid, status);
	free_arr(data.commands);
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
		return ;
	}
	cmd_arr = ft_split(clean_cmd, ' ');
	if (!cmd_arr || !*cmd_arr)
		exit(EXIT_FAILURE);
	int i = 0;
	char *clean_cmd2;
	while (cmd_arr[i])
	{
		clean_cmd2 = remove_quotes_first_word(cmd_arr[i]);
		if (!clean_cmd2)
		{
			printf("%s: The command not found\n", cmd_arr[i]);
			return ;
		}
		free(cmd_arr[i]);
		cmd_arr[i] = clean_cmd2;
		i++;
	}
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
			*status = WEXITSTATUS(wstatus);
		else if (WIFSIGNALED(wstatus))
			*status = 128 + WTERMSIG(wstatus);
	}
	else
	{
		perror("fork");
		*status = 1;
		exit(1);
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
	int	 i;

	if (is_empty(*line))
		return ;
	add_history(*line);
	if (check_quotes(*line) == 1)
		return ;
	check_quastion_sign(line, *status);
	bridge_var(line);
	if (!*line)
		return ;
	i = 0;
	while ((*line)[i] && ft_isspace((*line)[i]))
		i++;
	if ((*line)[i] == '\0')
		return ;
	if (ft_strcmp(*line + i, "clear") == 0 || ft_strncmp(*line + i, "clear ", 6) == 0)
		return (rl_clear_history());
	if (!ft_strchr(*line, '|'))
		return (execute_command_single(*line, status));
	execute_pipe_commands(*line, status);
}

int	main(void)
{
	char			*line;
	static int		status = 0;

	disable_ctrl_c_output(&status);
	setup_signal_handlers();
	while (1)
	{
		if (isatty(fileno(stdin)))
			line = readline("> ");
		else
		{
			line = get_next_line(fileno(stdin));
			line = ft_strtrim(line, "\n");
		}
		if (g_status == 130)
		{
			status = g_status;
			g_status = 0;
		}
		if (!line)
			handle_exit(ft_strdup("exit"), &status);
		else
			run_ex(&line, &status);
		free(line);
	}
	rl_clear_history();
}


// ps aux | grep bash | awk '{print $2}'    снова не работает		+
// Введите Ctrl+C внутри minishell (появляется лишний > в приглашении) +
// sleep 5

/* 
# Введите Ctrl+C внутри minishell (появляется лишний > в приглашении) +
sleep 5	(через раз)

ctrl + \ after some stuff should do nothing
если писать cat и затем нажимать ctrl+c появляется лишний > (иногда) +


> export PATH=/usr/bin					+
zsh: segmentation fault  ./minishell


> cat <filesfa    (виснит)			+
clean_filename: filesfa
filename: filesfa
open: No such file or directory

> 

> cat <<		(должна быть просто ошибка)	+

> | ls			(должна быть просто ошибка) +
> ls |			(должна быть просто ошибка) +
> ls || grep test							+

НЕ ДОЛЖНА ИНТЕРПРЕТИРОВАТЬ ЭТИ СИМВОЛЫ
echo hello; ls			+
echo hello \			+
echo \			+
echo -nnn hello			+

$nonexist | wc -l	+-
$nonexist			+-

> ./minishell
> cat << EOF
cat : Command not found
*/