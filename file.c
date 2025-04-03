#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	wr_stillquotes(char *line, int fd, int *i, char quotes)
{
	int	len;

	len = strlen(line);
	(*i)++;
	while (*i < len)
	{
		if (line[*i] == quotes)
			break ;
		write(fd, &line[*i], 1);
		(*i)++;
	}
	if (line[*i] == quotes)
		(*i)++;
}

void	mywrite(char *line, int fd)
{
	int		i;
	int		len;

	i = 0;
	len = strlen(line);
	while (i < len)
	{
		if (line[i] == '\'' || line[i] == '\"')
			wr_stillquotes(line, fd, &i, line[i]);
		else
		{
			write(fd, &line[i], 1);
			i++;
		}
	}
}

int main()
{
	mywrite("sex on the beach \"'\"hello\"'\"\n", 1);
}