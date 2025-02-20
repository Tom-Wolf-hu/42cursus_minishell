#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_var_name_size(char *str)
{
	int start = 0;
	int end = 0;

	while (str[start] && str[start] != '$')
		start++;
	if (str[start] != '$')
		return 0;
	start++;
	end = start;
	while (str[end] && str[end] != ' ' && str[end] != '$' && str[end] != '=' && str[end] != '\0')
		end++;
	return (end - start);
}

void get_var_name(char *dest, char *str)
{
	int start = 0;
	int end = 0;
	int i = 0;

	while (str[start] != '\0' && str[start] != '$')
		start++;
	if (str[start] != '$')
		return;
	start++;
	end = start;
	while (str[end] && str[end] != ' ' && str[end] != '\0' && str[end] != '$' && str[end] != '=')
		end++;
	while (start < end)
	{
		dest[i] = str[start];
		i++;
		start++;
	}
	dest[i] = '\0';
}

int cmp_names(char *name1, char *name2)
{
	int i = 0;

	while (name1[i] && name2[i] && name1[i] != '=' && name2[i] != '=')
	{
		if (name1[i] != name2[i])
			return 0;
		i++;
	}
	if ((name1[i] == '=' && name2[i] == '\0') || (name1[i] == '\0' && name2[i] == '\0'))
		return 1;
	return 0;
}

char *get_name(char *str)
{
	int i = 0;
	char *result;
	int j;

	while (str[i] && str[i] != '=')
		i++;
	result = malloc(i + 1);
	if (!result)
		return NULL;
	j = 0;
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[j] = '\0';
	return result;
}

char *find_var_value(char *var_name)
{
	extern char **environ;
	int i = 0;
	char *value;

	while (environ[i])
	{
		if (cmp_names(environ[i], var_name))
		{
			value = environ[i];
			while (*value && *value != '=')
				value++;
			if (*value == '=')
				return value + 1;
		}
		i++;
	}
	return NULL;
}

int	ft_strlen(char *str)
{
	int i = 0;

	while (str[i])
		i++;
	return (i);
}

void	change_str(char **str, char *name, char *value)
{
	int	i = 0;
	int	start = 0;
	int end = 0;
	char	*result;
	int		len = ft_strlen(*str) - ft_strlen(name) + ft_strlen(value) + 1; // i didnt minus dollar space, cause its space will be now for the '\0'

	result = malloc(len);
	if (!result)
		return ;
	while ((*str)[start] && (*str)[start] != '$')
	{
		result[start] = (*str)[start];
		start++;
	}
	end = start; // запомнить индекс для result
	start++; // пропускаем $
	while (i < ft_strlen(value)) // переписываем value
	{
		result[end] = value[i];
		end++;
		i++;
	}
	result[end] = '\0';
	// printf("%s %d\n", result, ft_strlen(result));
	i = end; // теперь индекс для result
	while ((*str)[start] && (*str)[start] != ' ' && (*str)[start] != '$' && (*str)[start] != '=' && (*str)[start] != '\0')
		start++;
	// printf("%d\n", start);
	while ((*str)[start] && i < len)
	{
		result[i] = (*str)[start];
		i++;
		start++;
	}
	result[i] = '\0';
	// printf("%s\n", result);
	free(*str);
	*str = result;
}

int bridge_var(char **str)
{
	char *var_name;
	int size;
	char *var_value;

	size = get_var_name_size(*str);
	if (size < 1)
	{
		printf("No variable found\n");
		return 0;
	}
	var_name = malloc(size + 1);
	if (!var_name)
		return 0;
	get_var_name(var_name, *str);
	var_value = find_var_value(var_name);
	if (var_value)
	{
		change_str(str, var_name, var_value);
		printf("%s\n", *str);
		return (free(var_name), 1);
	}
	printf("Variable not found in environ\n");
	return (free(var_name), 0);
}

int main()
{
    char *str = strdup("echo $USER hello");  // Можешь заменить на $PS или $PATH
    bridge_var(&str);
	free(str);
}
