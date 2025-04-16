/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/04/16 14:44:49 by omalovic         ###   ########.fr       */
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

void	wait_for_last_pid(pid_t last_pid, int *status)
{
	pid_t	wpid;
	int		wstatus;

	wpid = waitpid(-1, &wstatus, 0);
	while (wpid > 0)
	{
		if (wpid == last_pid)
		{
			if (WIFEXITED(wstatus))
				*status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
				*status = 128 + WTERMSIG(wstatus);
		}
		wpid = waitpid(-1, &wstatus, 0);
	}
}

char	**process_command_args(char **cmd_args, char *cmd)
{
	int		j;
	char	*clean_cmd2;

	j = 0;
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
	return (cmd_args);
}

void	save_and_redirect(struct s_saved_std *std, char *command, int *status)
{
	std->saved_stdin = dup(STDIN_FILENO);
	std->saved_stdout = dup(STDOUT_FILENO);
	handle_redirection(command, status);
}

void	close_saved_std(struct s_saved_std *std)
{
	close(std->saved_stdin);
	close(std->saved_stdout);
}

void	handle_child_process(t_pipe_data data, char *cmd, int i, int *status)
{
	char				**cmd_args;
	char				*clean_cmd;
	char				*path;
	struct s_saved_std	std;

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
		return (execute_builtin(data.commands[i], 1, status), exit(*status));
	clean_cmd = remove_redirects(data.commands[i]);
	cmd_args = ft_split(clean_cmd, ' ');
	save_and_redirect(&std, data.commands[i], status);
	cmd_args = process_command_args(cmd_args, cmd);
	path = get_command_path(cmd_args[0]);
	if (!path)
		return (printf("%s: Command not found\n", cmd), exit(127));
	close_saved_std(&std);
	return (execve(path, cmd_args, NULL), perror("execve"), exit(EXIT_FAILURE));
}

void	handle_parent_process(struct s_pipe_data *data,
		int i, pid_t pid, int *last_pid)
{
	if (data->prev_fd != -1)
		close(data->prev_fd);
	if (i < data->num_commands - 1)
		close(data->pipefd[1]);
	data->prev_fd = data->pipefd[0];
	if (i == data->num_commands - 1)
		*last_pid = pid;
}

void	hanlde_pid(struct s_pipe_data *data, char *cmd,
		int *status, int *last_pid)
{
	pid_t	pid;

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
	struct s_pipe_data	data;

	temp = NULL;
	ft_bzero(&data, sizeof(data));
	data.commands = get_commands(cmd, temp);
	if (temp)
		free(temp);
	if (!data.commands)
		return ;
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

void	handle_empty_command(char *cmd, int *status)
{
	struct s_saved_std	std;
	char				*line;

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
}

int	clean_command_args(char **cmd_arr)
{
	int		i;
	char	*clean_cmd2;

	i = 0;
	while (cmd_arr[i])
	{
		clean_cmd2 = remove_quotes_first_word(cmd_arr[i]);
		if (!clean_cmd2)
		{
			printf("%s: The command not found\n", cmd_arr[i]);
			free_arr(cmd_arr);
			return (1);
		}
		free(cmd_arr[i]);
		cmd_arr[i] = clean_cmd2;
		i++;
	}
	return (0);
}

void	restore_std(struct s_saved_std *std)
{
	dup2(std->saved_stdin, STDIN_FILENO);
	dup2(std->saved_stdout, STDOUT_FILENO);
	close(std->saved_stdin);
	close(std->saved_stdout);
}

void	execute_forked_process(char *path, char **cmd_arr, int *status)
{
	pid_t	pid;
	int		wstatus;

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
		return (perror("fork"), *status = 1, exit(1));
}

char	**split_and_clean_args(char *clean_cmd, int *status)
{
	char	**cmd_arr;

	cmd_arr = ft_split(clean_cmd, ' ');
	if (!cmd_arr || !*cmd_arr)
		exit(EXIT_FAILURE);
	if (clean_command_args(cmd_arr))
	{
		*status = 1;
		return (NULL);
	}
	return (cmd_arr);
}

void	execute_command_single(char *cmd, int *status)
{
	char				**cmd_arr;
	char				*path;
	struct s_saved_std	std;
	char				*clean_cmd;

	cmd_arr = NULL;
	if (is_builtin(cmd))
		return (execute_builtin(cmd, 1, status));
	clean_cmd = remove_redirects(cmd);
	if (clean_cmd && is_empty(clean_cmd))
	{
		free(clean_cmd);
		clean_cmd = NULL;
	}
	if (!clean_cmd)
		return (handle_empty_command(cmd, status));
	cmd_arr = split_and_clean_args(clean_cmd, status);
	if (!cmd_arr)
		return ;
	path = get_command_path(cmd_arr[0]);
	if (!path)
	{
		printf("%s: Command not found\n", clean_cmd);
		return (*status = 127, (void)0);
	}
	save_and_redirect(&std, cmd, status);
	if (*status == 1)
		return (restore_std(&std));
	execute_forked_process(path, cmd_arr, status);
	restore_std(&std);
	free_arr(cmd_arr);
}

void	run_ex(char **line, int *status)
{
	char	**arr;
	char	*clean_cmd;
	char	*new_line;
	int		i;

	if (is_empty(*line))
		return ;
	add_history(*line);
	if (check_quotes(*line) == 1)
		return ;
	// printf("before check_quastion_sign\n");
	if (check_quastion_sign(line, *status))
		return ;
	printf("after check_quastion_sign\n");
	bridge_var(line);
	printf("after bridge_var\n");

	if (!*line)
		return ;
	i = 0;
	while ((*line)[i] && ft_isspace((*line)[i]))
		i++;
	if ((*line)[i] == '\0')
		return ;
	if (ft_strcmp(*line + i, "clear") == 0
		|| ft_strncmp(*line + i, "clear ", 6) == 0)
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
			handle_exit(ft_strdup("exit"), &status, NULL);
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

echo "hello"$
*/