/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:43:14 by omalovic          #+#    #+#             */
/*   Updated: 2024/11/11 16:16:28 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	free_str(src1);
	return (result);
}

char	*get_remainder(char *buffer, int checker_result, int bytes_read)
{
	char	*remainder = NULL;
	int	i;
	int	size;

	i = 0;
	size = bytes_read - checker_result;
	if (size <= 0)
		return (NULL);
	remainder = (char *)malloc(sizeof(char) * (size + 1));
	if (!remainder)
		return (NULL);
	while (i < size)
	{
		remainder[i] = buffer[checker_result + i];
		i++;
	}
	remainder[size] = '\0';
	return (remainder);
}

char	*get_next_line(int fd)
{
	ssize_t	bytes_read;
	char	buffer[BUFFER_SIZE + 1];
	char	*result = NULL;
	static char *remainder = NULL;
	int	checker_result;

	if (remainder)
	{
		checker_result = checker(remainder);
		if (checker_result != -1)
		{
			result = gen_str(NULL, remainder, checker_result);
			remainder = get_remainder(remainder, checker_result, ft_strlen(remainder));
			return (result);
		}
		result = gen_str(NULL, remainder, ft_strlen(remainder));
		free(remainder);
		remainder = NULL;
	}
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (NULL);
		if (bytes_read == 0)
		{
			if (result && ft_strlen(result) > 0)
				return (result);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		checker_result = checker(buffer);
		if (checker_result != -1)
		{
			remainder = get_remainder(buffer, checker_result, bytes_read);
			result = gen_str(result, buffer, checker_result);
			return (result);
		}
		result = gen_str(result, buffer, bytes_read);
	}
	return (NULL);
}

/* int main()
{
	int fd = open("/Users/omalovic/get_next_line_project/file", O_RDONLY);
	if (fd == -1)
		return 1;
	printf("%s", get_next_line(fd));
	// printf("%s", get_next_line(fd));
	// printf("%s", get_next_line(fd));
	// printf("%s", get_next_line(fd));
	// printf("%s", get_next_line(fd));
	// printf("%s", get_next_line(fd));

} */