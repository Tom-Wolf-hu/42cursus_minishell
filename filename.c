#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_filename(char *cmd)
{
	char *filename;
	int j;
	int i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '>' || cmd[i] == '<')
			break ;
		i++;
	}
	while (cmd[i] && (cmd[i] == '>' || cmd[i] == '<'))
		i++;
	while (cmd[i] && (cmd[i] == 32 || cmd[i] == '\t' || cmd[i] == '\n'))
		i++;
	j = 0;
	filename = malloc(strlen(cmd) - i + 1);
	if (!filename)
		return (NULL);
	while (cmd[i])
	{
		filename[j] = cmd[i];
		i++;
		j++;
	}
	filename[j] = '\0';
	return (filename);
}

int main()
{
	char *name = get_filename("cat  filename");
	printf("%s\n", name);
}