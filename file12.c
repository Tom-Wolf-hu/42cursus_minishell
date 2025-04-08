#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void	remove_chars(char **str, char ch)
{
	char *new_str = NULL;
	int i = 0;
	int len = 0;
	int j = 0;

	// printf("i'm in func remove_chars\n");
	if (!*str || !str || ch == '\0')
		return ;
	// printf("str before: %s\n", *str);
	while ((*str)[i])
    {
        if ((*str)[i] != ch)
            len++;
        i++;
    }
	new_str = malloc(len + 1);
	if (!new_str)
		return ;
	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] != ch)
		{
			new_str[j] = (*str)[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	// printf("str after: %s\n", new_str);
	free(*str);
	*str = new_str;
}

int main()
{

	char *str = strdup("'\"'Hello world!'\"'");
	remove_chars(&str, '\'');
	if (str)
	printf("%s\n", str);
}