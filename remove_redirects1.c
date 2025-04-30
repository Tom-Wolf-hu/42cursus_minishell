#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int check_quotes_arg(char *line)
{
	int		i;
	char	first_quote;

	i = 0;
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] == '\'' || line[i] == '\"')
	{
		first_quote = line[i];
		i++;
	}
	while (line[i] && first_quote)
	{
		if (isspace(line[i]))
			return (0);
		if (line[i] == first_quote)
			return (1);
		i++;
	}
	return (0);
}

int	check_empty_cmd(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] == '\'' || line[i] == '\"')
	{
		quote = line[i];
		i++;
	}
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] == quote || line[i] == '\0')
		return (0);
	return (1);
}

int	no_first_quotes(char *line)
{
	int	i;

	i = 0;
	while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != '\'' && line[i] != '\"')
		return (1);
	return (0);
}

char	*copy_str(char **result, char *line)
{
	int	first_quote;
	int	i;
	int	j;

	i = 0;
	while (line[i] && isspace(line[i]))
		i++;
	i++;
	j = 0;
	first_quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '\"') && !first_quote)
		{
			first_quote = 1;
			i++;
			continue ;
		}
		(*result)[j] = line[i];
		i++;
		j++;
	}
	(*result)[j] = '\0';
	return ((*result));
}

char	*remove_quotes_first_word(char *line)
{
	char	*result;
	int		i;
	int		j;
	int		first_quote;

	if (no_first_quotes(line))
		return (strdup(line));
	if (!check_quotes_arg(line) || !check_empty_cmd(line))
		return (NULL);
	result = malloc(strlen(line) - 2 + 1);
	if (!result)
		return (NULL);
	return (copy_str(&result, line));
}

int main()
{
	// printf("%d\n", check_quotes_arg("\"echo\"       hello worold\""));
	printf("%s\n", remove_quotes_first_word("   \"\"  \"echo\"       \"hello worold\""));
	// printf("%d\n", check_empty_cmd("     \"   \"  \"echo\"       hello worold\""));
}