/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:43:14 by omalovic          #+#    #+#             */
/*   Updated: 2024/10/30 13:31:37 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_all(char *buffer_1, char *buffer_2)
{
	free (buffer_1);
	free (buffer_2);
	return (NULL);
}

static char	*ft_strjoin_free(char *str1, char *str2)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	*str_final;

	i = ft_strlen(str1);
	j = 0;
	if (str2)
		j = ft_strlen(str2);
	k = 0;
	if (i + j == 0)
		return (free_all(str1, str2));
	str_final = (char *)ft_calloc(i + j + 1, sizeof(char));
	while (k < i && str_final)
	{
		str_final[k] = str1[k];
		++k;
	}
	while (k < i + j && str2 && str_final)
	{
		str_final[k] = str2[k - i];
		++k;
	}
	free_all(str1, str2);
	return (str_final);
}

static char	*get_next_line_with_buff(int fd, int *err_flag)
{
	char	*curr_buffer;
	char	*received_buffer;
	int		read_chars;

	curr_buffer = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!curr_buffer)
		return (*err_flag = 1, NULL);
	read_chars = read(fd, curr_buffer, BUFFER_SIZE);
	if (read_chars < 0)
		return (*err_flag = 1, free(curr_buffer), NULL);
	if (ft_strchr(curr_buffer, '\n') || read_chars < BUFFER_SIZE)
		return (curr_buffer);
	received_buffer = get_next_line_with_buff(fd, err_flag);
	if (*err_flag)
		return (free_all(received_buffer, curr_buffer));
	if (!received_buffer)
		return (curr_buffer);
	curr_buffer = ft_strjoin_free(curr_buffer, received_buffer);
	if (!curr_buffer)
		return (*err_flag = 1, NULL);
	return (curr_buffer);
}

static char	*separate_line_and_buffer(char **line_and_buffer)
{
	char	*newline_pos;
	char	*new_line;
	char	*new_buffer;
	size_t	new_line_len;

	newline_pos = ft_strchr(*line_and_buffer, '\n');
	new_line_len = newline_pos - *line_and_buffer;
	if (!newline_pos)
	{
		*line_and_buffer = ft_strjoin_free(*line_and_buffer, NULL);
		return (NULL);
	}
	new_line = (char *)ft_calloc(new_line_len + 2, sizeof(char));
	new_buffer = (char *)ft_calloc(ft_strlen(newline_pos), sizeof(char));
	if (!new_buffer || !new_line)
		return (free_all(new_buffer, new_line));
	ft_strlcpy(new_buffer, newline_pos + 1, ft_strlen(newline_pos));
	ft_strlcpy(new_line, *line_and_buffer, new_line_len + 2);
	free (*line_and_buffer);
	*line_and_buffer = new_line;
	if (ft_strcmp(new_buffer, ""))
		return (new_buffer);
	free(new_buffer);
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*persistent_buffer;
	char		*curr_line;
	int			err_flag;

	err_flag = 0;
	curr_line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!persistent_buffer || !ft_strchr(persistent_buffer, '\n'))
		curr_line = get_next_line_with_buff(fd, &err_flag);
	if (!curr_line && (err_flag || !persistent_buffer))
	{
		free (persistent_buffer);
		persistent_buffer = NULL;
		return (NULL);
	}
	if (persistent_buffer)
	{
		curr_line = ft_strjoin_free(persistent_buffer, curr_line);
		if (!curr_line)
			return (persistent_buffer = NULL);
	}
	persistent_buffer = separate_line_and_buffer(&curr_line);
	return (curr_line);
}

/* int main()
{
	int fd = open("/Users/omalovic/get_next_line_train/file.txt", O_RDONLY);
	printf("%s\n", get_next_line(fd));
	printf("%s\n", get_next_line(fd));
	printf("%s\n", get_next_line(fd));
	printf("%s\n", get_next_line(fd));
	printf("%s\n", get_next_line(fd));
} */