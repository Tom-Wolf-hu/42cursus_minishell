#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

char	*find_cmd_path(char *cmd)
{
	char	*path = getenv("PATH");
	char *dir;
	char *full_path;

	if (!path)
		return (NULL);
	dir = strtok(path, ":");
	printf("%s\n", dir);
	while (dir)
	{
		full_path = malloc(strlen(dir) + strlen(cmd) + 2);
		if (!full_path)
			return (NULL);
		sprintf(full_path, "%s/%s", dir, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		dir = strtok(NULL, ":");
	}
	return (NULL);
}

int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int main()
{
	// printf("%s\n", find_cmd_path("ls"));
	printf("%d\n", ft_isspace('t'));
}