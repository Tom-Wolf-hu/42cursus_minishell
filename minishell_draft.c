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

void	save_std(struct s_saved_std *std)
{
	std->saved_stdout = dup(STDOUT_FILENO);
	if (std->saved_stdout == -1)
	{
		perror("Failed duplicate STDOUT");
		exit(EXIT_FAILURE);
	}
	std->saved_stdin = dup(STDIN_FILENO);
	if (std->saved_stdin == -1)
	{
		perror("Failed duplicate STDIN");
		exit(EXIT_FAILURE);
	}
}

void	restore_fd(struct s_saved_std *std, int opt)
{
	if (opt == 1)
	{
		if (dup2(std->saved_stdin, STDIN_FILENO) == -1)
		{
			perror("Failed to restore STDIN");
			exit(EXIT_FAILURE);
		}
		if (dup2(std->saved_stdout, STDOUT_FILENO) == -1)
		{
			perror("Failed to restore STDOUT");
			exit(EXIT_FAILURE);
		}
	}
	close(std->saved_stdin);
	close(std->saved_stdout);
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
		save_std(&std);
		handle_redirection(cmd, status);
		if (*status == 1)
		{
			restore_fd(&std, 1);
			return ;
		}
		line = get_next_line(STDIN_FILENO);
		while (line)
		{
			write(STDOUT_FILENO, line, ft_strlen(line));
			free(line);
			line = get_next_line(STDIN_FILENO);
		}
		restore_fd(&std, 1);
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
	save_std(&std);
	handle_redirection(cmd, status);
	if (*status == 1)
	{
		restore_fd(&std, 1);
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
	restore_fd(&std, 1);
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