#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
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
			// free_arr(path_arr);
			return (NULL);
		}
		if (access(buffer, X_OK) == 0)
		{
			// free_arr(path_arr);
			return (buffer);
		}
		free(buffer);
		i++;
	}
	if (buffer)
		free(buffer);
	// free_arr(path_arr);
    return NULL;
}

void	execute_command_single(char *cmd)
{
	char **cmd_arr;
	char *path;
	int pid;

	cmd_arr = ft_split(cmd, ' ');
	if (!cmd_arr || !*cmd_arr)
		exit(EXIT_FAILURE);
	path = get_command_path(cmd_arr[0]);
	if (!path)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == 0)
	{
		if (!path)
			exit(EXIT_FAILURE);
		printf("%s\n", path);
		execve(path, cmd_arr, NULL);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		printf("cmd has worked!\n");
	}
	else
		perror("fork");
	// free_arr(cmd_arr);
}

// int main()
// {
//     char *cmd = "rm file2.o";
//     char *cmd = "cat file.txt | grep pattern | wc -l";
//     execute_command_single(cmd);
// }

// -----------------PIPES------------------

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
				execute_builtin(commands[i], fd, status);
				exit(EXIT_SUCCESS);
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
		prev_fd = pipefd[0];
		waitpid(pid, NULL, 0);
	}
	for (i = 0; i < num_commands; i++)
		free(commands[i]);
	free(commands);
}

int main()
{
    char *cmd = "cat file.txt | wc -l";  // Пример команды с несколькими пайпами
    execute_pipe_commands(cmd);
    return 0;
}
