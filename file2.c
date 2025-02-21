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

void change_to_exit_status(int i, char *line, char *status)
{
	
}

int	check_quastion_sign(char **line, char *status)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (line[i + 1] == '?')
			{
				change_to_exit_status(i, line, status);
			}
		}
		i++;
	}
}

int main()
{
	char *line = strdup("hello $?");
	check_quastion_sign(&line, "0");
	printf("%s\n", line);
}