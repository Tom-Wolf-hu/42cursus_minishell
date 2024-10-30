/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:43:10 by omalovic          #+#    #+#             */
/*   Updated: 2024/10/30 13:32:08 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	ret_value;

	ret_value = 0;
	while (*s1 != '\0' && *s2 != '\0')
	{
		ret_value = *s1 - *s2;
		if (ret_value != 0)
			return (ret_value);
		++s1;
		++s2;
	}
	ret_value = *s1 - *s2;
	return (ret_value);
}

int	ft_strlen(const char *str)
{
	int	strlen;

	strlen = 0;
	while (*str != '\0')
	{
		++strlen;
		++str;
	}
	return (strlen);
}

void	*ft_calloc(size_t num, size_t size)
{
	void	*return_ptr;
	char	*tmp_ptr;
	size_t	len;

	len = num * size;
	return_ptr = malloc (len);
	if (!return_ptr)
		return (NULL);
	tmp_ptr = (char *) return_ptr;
	while (len--)
	{
		*tmp_ptr = '\0';
		++tmp_ptr;
	}
	return (return_ptr);
}

char	*ft_strchr(const char *s, int c)
{
	char	ch;

	ch = (char) c;
	while (*s)
	{
		if (*s == ch)
			return ((char *) s);
		++s;
	}
	if (*s == ch)
		return ((char *) s);
	return (NULL);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	int		ret_value;
	size_t	i;

	i = 0;
	ret_value = 0;
	while (src[ret_value])
		ret_value++;
	while (i < size - 1 && src[i] && size)
	{
		dest[i] = src[i];
		++i;
	}
	if (size)
		dest[i] = '\0';
	return (ret_value);
}
