#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_quotes(char *str)
{
	int i = 0;
	char *result;
	int quote_closed = 0;
	int in_quote = 0;

	while (str[i])
	{
		if (str[i] == '\"' && !in_quote)
		{
			i++;
			in_quote = 1;
			continue;
		}
		if (str[i] == '\"' && in_quote)
		{
			i++;
			in_quote = 0;
			continue;
		}
		write(1, &str[i], 1);
		i++;
	}
}

int main()
{
    char *str = "echo \"hello\"";
	remove_quotes(str);
	// printf("%s\n", remove_quotes(str));
}
