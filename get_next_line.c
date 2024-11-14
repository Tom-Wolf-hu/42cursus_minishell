/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:43:14 by omalovic          #+#    #+#             */
/*   Updated: 2024/11/14 17:47:23 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// if (remainder)
// {
// 	checker_result = checker(remainder);
// 	if (checker_result != -1)
// 	{
// 		result = gen_str(NULL, remainder, checker_result);
// 		remainder = get_remainder(remainder, checker_result, ft_strlen(remainder));
// 		free_str(remainder);
// 		return (result);
// 	}
// 	result = gen_str(NULL, remainder, ft_strlen(remainder));
// 	free_str(remainder);
// 	remainder = NULL;
// }

#include "get_next_line.h"

char	*ft_copystr(char **remainder, int checker_result)
{
	char	*result;
	int	i;

	if (!*remainder || checker_result <= 0)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (checker_result + 1));
	if (!result)
	{
		free_str(*remainder);
		*remainder = NULL;
		return (NULL);
	}
	i = 0;
	while (i < checker_result)
	{
		result[i] = (*remainder)[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*gen_str(char *src1, char *src2, int i2)
{
	int		i1;
	char	*result;
	int		j;
	int		k;

	if (!src2)
		return (NULL);
	if (!src1)
		return (ft_copystr(&src2, i2));
	i1 = ft_strlen(src1);
	if (i1 == -1)
		return (NULL);
	if (i1 < 0 || i2 < 0)
		return (NULL);
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
	char	*remainder;
	int	i;
	int	size;

	size = bytes_read - checker_result;
	if (size <= 0)
		return (NULL);
	remainder = (char *)malloc(sizeof(char) * (size + 1));
	if (!remainder)
		return (NULL);
	i = 0;
	while (i < size)
	{
		remainder[i] = buffer[checker_result + i];
		i++;
	}
	remainder[size] = '\0';
	return (remainder);
}

int	deal_with_remainder(char **remainder, char **result)
{
	char *new_remainder;
	
	if (*remainder)
	{
		if (checker(*remainder) != -1)
		{
			*result = gen_str(NULL, *remainder, checker(*remainder));
			new_remainder = get_remainder(*remainder, checker(*remainder), ft_strlen(*remainder));
			free_str(*remainder);
			*remainder = new_remainder;
			if (!*remainder)
			{
				if (*result)
					return (1);
				return (-1);
			}
			return (1);
		}
		*result = ft_copystr(remainder, ft_strlen(*remainder));
		if (!*result)
			return (-1);
		free_str(*remainder);
		*remainder = NULL;
	}
	return (0);
}

char	*get_next_line(int fd)
{
	ssize_t		bytes_read;
	char		buffer[BUFFER_SIZE + 1];
	char		*result;
	static char	*remainder = NULL;

	result = NULL;
	if (deal_with_remainder(&remainder, &result) == 1)
		return (result);
	if (deal_with_remainder(&remainder, &result) == -1)
		return (NULL);
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free_str(remainder), free_str(result), NULL);
		if (bytes_read == 0)
		{
			if (result && ft_strlen(result) > 0)
				return (result);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		if (checker(buffer) != -1)
		{
			result = gen_str(result, buffer, checker(buffer));
			remainder = get_remainder(buffer, checker(buffer), bytes_read);
			if (!remainder)
			{
				if (result)
					return (result);
				return (NULL);
			}
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
	// printf("firstcall:\n%s\nend of first call...\n", get_next_line(fd));
	// printf("secondcall:\n%s\nend of second call...\n", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));

} */