#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_filename(char *cmd)
{
	char *filename;
	int i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '>')
			break ;
		i++;
	}
	if (cmd[i] == '>')
		i++;
	while (cmd[i] && cmd[i] == ' ' && cmd[i] == '\t' && cmd[i] == '\n')
		i++;
	filename = strdup(cmd + i);
	return (filename);
}

int main()
{
	char *str = get_filename("echo hello > file.txt");
	if (str)
		printf("%s\n", str);
}