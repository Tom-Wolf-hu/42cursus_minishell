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

char	*copy_quoted_word(char *line, char *result, int i, char quote_char)
{
	int	j;

	j = 0;
	while (line[i] && line[i] != quote_char)
		result[j++] = line[i++];
	if (line[i] == quote_char)
		i++;  // Пропускаем закрывающую кавычку
	result[j] = '\0';
	return (result);
}

char	*remove_quotes_first_word(char *line)
{
	int		i;
	char	quote_char;
	char	*result;
	int		len;

	i = 0;
	if (!line)
		return (NULL);
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != '\'' && line[i] != '\"')
		return (strdup(line));
	quote_char = line[i++];
	len = 0;
	while (line[i + len] && line[i + len] != quote_char)
		len++;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	return (copy_quoted_word(line, result, i, quote_char));
}

int main()
{
	char *str = remove_quotes_first_word("'echo' \"cat lol.c | cat > lol.c\"");
	printf("%s\n", str);
}
