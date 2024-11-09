/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:43:14 by omalovic          #+#    #+#             */
/*   Updated: 2024/11/09 16:34:22 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	free_str(char *str, int sign)
{
	if (sign)
		free(str);
	if (!sign)
	{
		if (str != NULL)
			free(str);
	}
}

int	get_i1(char *str)
{
	if (str != NULL)
		return (ft_strlen(str));
	return (0);
}

char	*gen_str(char *src1, char *src2, int i2)
{
	int		i1;
	char	*result;
	int		j;
	int		k;

	i1 = get_i1(src1);
	result = (char *)malloc(sizeof(char) * (i1 + i2 + 1));
	if (!result)
		return (NULL);
	j = 0;
	while (j < i1)
	{
		result[j] = src1[j];
		j++;
	}
	k = 0;
	while (k < i2)
	{
		result[j] = src2[k];
		j++;
		k++;
	}
	result[j] = '\0';
	free_str(src1, 0);
	return (result);
}

int	checker(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (i);
}

char	*get_next_line(int fd)
{
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE + 1];
	char	*result = NULL;
	static char remainder;

	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			return (NULL);
		}
		if (bytes_read == 0) // срабатывает это условие
			return (NULL);
		buffer[bytes_read] = '\0';
		result = gen_str(result, buffer, checker(buffer));
		if (!result)
		{
			return (NULL);
		}
		if (checker(buffer) < (int)bytes_read)
		{
			return (result);
		}
	}
	return (NULL);
}

int main()
{
	int fd = open("/Users/omalovic/get_next_line_project/file", O_RDONLY);
	if (fd == -1)
		return 1;
	printf("%s\n", get_next_line(fd));
	printf("%s\n", get_next_line(fd));
	printf("%s\n", get_next_line(fd));


}