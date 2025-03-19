#include "../minishell.h"

void	execute_command_single(char *cmd, int *status)
{
	char **cmd_arr;
	char *path;
	int pid;
	int wstatus;

	if (is_builtin(cmd))
		return (execute_builtin(cmd, 1, status));
	//////////////////////////////////////////////
	wstatus = handle_redirection(cmd);
	if (wstatus)
		return ;
	if (wstatus == -1)
		exit(EXIT_FAILURE);
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

int	handle_redirection(char *cmd)
{
	char **commands = ft_split(cmd, ' ');
	if (!commands || !*commands)
		return -1;
	
}


int main()
{
	char *str = ft_strdup("cat 'hello' > file");
	printf("%d\n", handle_redirection(str));
}
