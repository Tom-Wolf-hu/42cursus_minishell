#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *remove_quotes_first_word(char *str, char ch)
{
	int i;
	int j;
	char *result;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	if (str[i] == ch)
	{
		result = malloc(strlen(str) - 2 + 1);
		if (!result)
			return (NULL);
		i++;
		while (str[i] && str[i] != ch)
		{
			result[j] = str[i];
			j++;
			i++;
		}
		i++;
		while (str[i])
		{
			result[j] = str[i];
			j++;
			i++;
		}
		result[j] = '\0';
		return (result);
	}
	return (strdup(str));
}

int main()
{
	char *str = "'cat' hello";
	char *newstr = remove_quotes_first_word(str, '\'');
	if (newstr)
		printf("result: %s\n", newstr);
}