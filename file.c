#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *wr_stillquotes(char *line, int *i, char quotes)
{
    int len;
    int start;
    int end;
	char *result;

	len = strlen(line);
	start = *i + 1;
	end = start;
    while (end < len)
	{
        if (line[end] == '\\' && quotes == '\"' && end + 1 < len && (quotes == '\"' || quotes == '\''))
            end += 2;
        else if (line[end] == quotes)
            break;
		else
            end++;
    }
    result = (char *)malloc((end - start + 1) * sizeof(char));
    if (!result)
		return NULL;
    memcpy(result, line + start, end - start);
    result[end - start] = '\0';
    if (end < len && line[end] == quotes)
		*i = end + 1;
	else
		*i = end;
    return result;
}

char *remove_quotes(char *line)
{
    int i;
	int len;
    char *result;
	int pos;
	char *tmp;

	i = 0;
	len = strlen(line);
	result = (char *)malloc(len + 1);
    if (!result)
		return NULL;
    pos = 0;
    while (i < len)
	{
        if (line[i] == '\'' || line[i] == '\"')
		{
            tmp = wr_stillquotes(line, &i, line[i]);
            if (tmp)
			{
                strcpy(result + pos, tmp);
                pos += strlen(tmp);
                free(tmp);
            }
        }
		else
            result[pos++] = line[i++];
    }
    result[pos] = '\0';
    return result;
}

int main()
{
	char *str = "\"\"cat\"\" hello";
	char *newstr = mywrite(str);
	if (newstr)
		printf("%s\n", newstr);
}