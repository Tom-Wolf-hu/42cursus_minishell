#include "minishell.h"


int check_command_quotes(char *line)
{
    int in_quote = 0;
    int i = 0;

    while (line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;
    if ((line[i] == '\'' || line[i] == '\"') && line[i + 1] == line[i])
        return 0;
    while (line[i]) 
    {
        if ((line[i] == '\'' || line[i] == '\"') && !in_quote) 
        {
            in_quote = 1;
        }
        else if ((line[i] == '\'' || line[i] == '\"') && in_quote) 
        {
            in_quote = 0;
        }
        if (isspace(line[i]) && in_quote)
            return 0;
        i++;
    }
    if (in_quote)
        return 0;
    return 1; // Если все правильно
}

char *remove_quotes_first_word(char *line)
{
	int i = 0;
	int j = 0;
	char *result;

    while (line[i] && isspace(line[i]))
		i++;
	if (line[i] != '\'' && line[i] != '\"')
		return (strdup(line));
	result = malloc(strlen(line) - 2 + 1);
	if (!result)
		return (NULL);
	i++;
	while (line[i] && line[i] != '\'' && line[i] != '\"')
	{
		result[j] = line[i];
		j++;
		i++;
	}
	if (line[i] == '\'' || line[i] == '\"')
		i++;
	while (line[i])
	{
		result[j] = line[i];
		j++;
		i++;
	}
	result[j] = '\0';
	return (result);
}

int main()
{
    // Пример тестирования
    printf("%d\n", check_command_quotes("\"echo \" \"hello\""));  // Ожидается -1
    printf("%d\n", check_command_quotes("\"echo\" \"hello\""));   // Ожидается 1
    printf("%d\n", check_command_quotes("\"echo\" hello"));       // Ожидается 1
    printf("%d\n", check_command_quotes("echo \"hello\""));       // Ожидается 1
    printf("%d\n", check_command_quotes("'' echo"));              // Ожидается -1
    printf("%d\n", check_command_quotes("\"\" echo"));            // Ожидается -1

	char line1[] = "\"echo\" hello world";
    char *new_str = remove_quotes_first_word(line1);
	if (new_str)
		printf("Result 1: %s\n", new_str);
    return 0;
}