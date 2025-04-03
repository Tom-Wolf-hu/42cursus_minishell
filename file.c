#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*join_str(char const *s1, char const *s2, char *result)
{
	unsigned int	index;
	unsigned int	len_s;

	index = 0;
	len_s = 0;
	while (s1[len_s] != '\0')
	{
		result[len_s] = s1[len_s];
		len_s++;
	}
	while (s2[index] != '\0')
	{
		result[len_s] = s2[index];
		index++;
		len_s++;
	}
	result[len_s] = '\0';
	return (result);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*result;
	unsigned int	len_s;
	unsigned int	general_len;

	if (!s1 || !s2)
		return (NULL);
	general_len = 0;
	len_s = 0;
	while (s1[len_s] != '\0')
		len_s++;
	while (s2[general_len] != '\0')
		general_len++;
	general_len = len_s + general_len;
	result = (char *)malloc(sizeof(char) * (general_len + 1));
	if (!result)
		return (NULL);
	return (join_str(s1, s2, result));
}

char *ft_join(char **arr)
{
	int i = 0;
	char *result;
	char *temp;

	if (!*arr)
		return (NULL);
	result = strdup(arr[i]);
	if (!result)
		return (NULL);
	while (arr[i + 1])
	{
		temp = ft_strjoin(result, " ");
		if (!temp)
			return (free(result), NULL);
		free(result);
		result = temp;
		temp = ft_strjoin(result, arr[i + 1]);
		if (!temp)
		{
			free(result);
			return (NULL);
		}
		free(result);
		result = temp;
		i++;
	}
	return (result);
}

int main()
{
	char *str[] = {"echo", "hello", "how", "are", "you?", NULL};
	char *new_str = ft_join(str);
	if (new_str)
		printf("%s\n", new_str);
}