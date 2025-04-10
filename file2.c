#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdbool.h>

extern char **environ;

void	free_arr2(char **arr, int till)
{
	int	i;

	i = 0;
	while (arr[i] && i < till)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**copy_environ(void)
{
	int		i;
	int		count;
	char	**my_environ;

	i = 0;
	count = 0;
	my_environ = NULL;
	while (environ[count])
		count++;
	my_environ = malloc(sizeof(char *) * (count + 1));
	if (!my_environ)
	{
		perror("malloc");
		return (exit(1), NULL);
	}
	while (environ[i])
	{
		my_environ[i] = strdup(environ[i]);
		if (!my_environ[i])
			return (free_arr2(my_environ, i), exit(1), NULL);
		i++;
	}
	my_environ[i] = NULL;
	return (my_environ);
}

int		handle_export(char *line, int fd)
{
	char	**my_environ;

	// my_environ = copy_environ();
	// if (ft_strlen(line) == 6 || check_line(line, 7))
	// 	return (print_env(fd));

}

int main()
{
	handle_export("line", 1);
}
