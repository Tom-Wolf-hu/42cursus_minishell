#include "../minishell.h"

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
			// if (handle_redirection(commands[i]) == -1) {
				// exit(EXIT_FAILURE);
			// }
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
	// if (handle_redirection(cmd) == -1) {
		// *status = 1;
		// return;
	// }
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
	// if (is_empty(*line))
		// return ;
	// add_history(*line);
	// check_quastion_sign(line, ft_itoa(*status));
	// bridge_var(line);
	if (!ft_strchr(*line, '|'))
		return (execute_command_single(*line, status));
	execute_pipe_commands(*line, 1, status);
	// if (is_builtin(*line))
		// execute_builtin(*line, 1, status);
}


int main()
{
	char *str = ft_strdup("ls");
	run_ex(&str, 0);
}
