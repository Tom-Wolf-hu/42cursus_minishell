#include "minishell.h"

// void	join_part(char **s1, char *s2)
// {
// 	int		lens1;
// 	int		lens2;
// 	char	*temp;
// 	int		i;

// 	lens1 = 0;
// 	i = 0;
// 	// fprintf(stderr, "s1: '%s'\ts2: '%s'\n", *s1, s2);
// 	if (*s1 != NULL)
// 		lens1 = strlen(*s1);
// 	if (s2 == NULL)
// 		return ;
// 	lens2 = strlen(s2);
// 	// fprintf(stderr, "lens1: %d\tlens2: %d\n", lens1, lens2);
// 	temp = (char *)malloc((lens1 + lens2 + 1) * sizeof(char));
// 	if (!temp)
// 	{
// 		perror("Failed to allocate memory for temp in join part");
// 		exit(EXIT_FAILURE);
// 	}
// 	while (*s1 && i < lens1)
// 	{
// 		temp[i] = (*s1)[i];
// 		// fprintf(stderr, "temp[%i] in while cycle: '%c'\n", i, temp[i]);
// 		i++;
// 	}
// 	// write(1, "1passed1\n", 9);
// 	// fprintf(stderr, "temp after s1 copy: %s\n", temp);
// 	i = 0;
// 	while (s2 && i < lens2)
// 	{
// 		temp[lens1 + i] = s2[i];
// 		i++;
// 	}
// 	// write(1, "1passed2\n", 9);
// 	// fprintf(stderr, "temp after s2 copy: %s\n", temp);
// 	// fprintf(stderr, "lens1 + i: %d\t lens1 + lens2 + 1: %d\n", lens1 + i, lens1 + lens2 + 1);
// 	temp[lens1 + i] = '\0';
// 	free(*s1);
// 	free(s2);
// 	s2 = NULL;
// 	*s1 = temp;
// 	// fprintf(stderr, "s1 in join_part: %s\n", *s1);
// }

// void	redir_part(char *cmd, int *i)
// {
// 	while (cmd[*i] && (cmd[*i] == '<' || cmd[*i] == '>' || isspace(cmd[*i])))
// 		(*i)++;
// 	while (cmd[*i] && !isspace(cmd[*i]))
// 		(*i)++;
// }

// char	*before_red(char *cmd, int *i)
// {
// 	int		start;
// 	int		j;
// 	char	*bef_red;

// 	start = *i;
// 	j = 0;
// 	while (cmd[*i] && cmd[*i] != '>' && cmd[*i] != '<')
// 		(*i)++;
// 	// printf("*i == %d\n", *i);
// 	// printf("ch == %c\n", cmd[*i]);
// 	if (start == *i)
// 		return (NULL);
// 	// if (start + 1 == *i || *i + 1 == start || start == *i)
// 	// 	return NULL;
// 	// printf("*i - start == %d\n", *i - start);
// 	bef_red = malloc((*i - start + 1));
// 	if (!bef_red)
// 	{
// 		perror("Failed to allocate memory for bef_red");
// 		exit(EXIT_FAILURE);
// 	}
// 	while (start < *i)
// 	{
// 		bef_red[j] = cmd[start];
// 		start++;
// 		j++;
// 	}
// 	bef_red[j] = '\0';
// 	return (bef_red);
// }

// char	*remove_redirects(char *cmd)
// {
// 	char	*clean_cmd;
// 	char	*temp;
// 	int		i;

// 	i = 0;
// 	clean_cmd = NULL;
// 	// write(1, "passed1\n", 8);
// 	while (cmd[i])
// 	{
// 		// write(1, "passed2\n", 8);
// 		temp = before_red(cmd, &i);
// 		// if (!temp)
// 		// {
// 		// 	printf("clean_cmd is null\n");
// 		// 	break ;
// 		// }
// 		// write(1, "passed3\n", 8);
// 		join_part(&clean_cmd, temp);
// 		// write(1, "passed4\n", 8);
// 		redir_part(cmd, &i);
// 	}
// 	// printf("clean_cmd in remove redirects: %s, %d\n", clean_cmd, ft_strlen(clean_cmd));
// 	return (clean_cmd);
// }

// char *get_temp_remove_quotes(char *line, int *i, char quotes)
// {
//     int len;
//     int start;
//     int end;
// 	char *result;

// 	len = strlen(line);
// 	start = *i + 1;
// 	end = start;
//     while (end < len)
// 	{
//         if (line[end] == '\\' && quotes == '\"' && end + 1 < len && (quotes == '\"' || quotes == '\''))
//             end += 2;
//         else if (line[end] == quotes)
//             break;
// 		else
//             end++;
//     }
//     result = (char *)malloc((end - start + 1) * sizeof(char));
//     if (!result)
// 		return NULL;
//     memcpy(result, line + start, end - start);
//     result[end - start] = '\0';
//     if (end < len && line[end] == quotes)
// 		*i = end + 1;
// 	else
// 		*i = end;
//     return result;
// }

// char *remove_quotes(char *line)
// {
//     int i;
// 	int len;
//     char *result;
// 	int pos;
// 	char *tmp;

// 	i = 0;
// 	len = strlen(line);
// 	result = (char *)malloc(len + 1);
//     if (!result)
// 		return NULL;
//     pos = 0;
//     while (i < len)
// 	{
//         if (line[i] == '\'' || line[i] == '\"')
// 		{
//             tmp = get_temp_remove_quotes(line, &i, line[i]);
//             if (tmp)
// 			{
//                 strcpy(result + pos, tmp);
//                 pos += strlen(tmp);
//                 free(tmp);
//             }
//         }
// 		else
//             result[pos++] = line[i++];
//     }
//     result[pos] = '\0';
//     return result;
// }

// int check_command_quotes(char *line)
// {
//     int in_quote = 0;
//     int i = 0;

//     while (line[i] && (line[i] == ' ' || line[i] == '\t'))
//         i++;
//     if ((line[i] == '\'' || line[i] == '\"') && line[i + 1] == line[i])
//         return 0;
//     while (line[i]) 
//     {
//         if ((line[i] == '\'' || line[i] == '\"') && !in_quote) 
//         {
//             in_quote = 1;
//         }
//         else if ((line[i] == '\'' || line[i] == '\"') && in_quote) 
//         {
//             in_quote = 0;
//         }
//         if (isspace(line[i]) && in_quote)
//             return 0;
//         i++;
//     }
//     if (in_quote)
//         return 0;
//     return 1;
// }

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
	if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
		return (NULL);
	while (line[i] && line[i] != '\'' && line[i] != '\"')
	{
		if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
			return (free(result), NULL);
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
	// char *str = "<file wc -l | grep test >file";
	// char *newstr = remove_quotes_commands(str);
	// if (newstr)
	// 	printf("%s\n", newstr);

	char *str = "\"echo\" hello";
	char *new_str = remove_quotes_first_word(str);
	if (new_str)
		printf("%s\n", new_str);
}

// char	*remove_quotes_commands(char *line)
// {
// 	char **arr;
// 	char *clean_result;
// 	char *clean_str;
// 	int i = 0;

// 	arr = ft_split(line, '|');
// 	while (arr[i])
// 	{
// 		clean_str = remove_redirects(arr[i]);
// 		if (!clean_str)
// 		{
// 			// free_arr(arr);
// 			return (NULL);
// 		}
// 		if (!check_command_quotes(clean_str))
// 		{
// 			free(clean_str);
// 			// free_arr(arr);
// 			return (NULL);
// 		}
// 		clean_str = remove_quotes_first_word(clean_str);
// 		if (!clean_str)
// 			return (NULL);
// 		// printf("%s\n", clean_str);
// 		free(arr[i]);
// 		arr[i] = clean_str;
// 		i++;
// 	}
// 	clean_result = ft_join(arr);
// 	return (free_arr(arr), clean_result);
// }



// USAGE: check_quastion_sign(line_to_check, char* status)
// добавить проверку для '