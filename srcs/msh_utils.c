/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:22:01 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/15 11:55:55 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_arr(char **strarr)
{
	int	i;

	i = 0;
	if (!strarr)
	{
		printf("The stringarray point to NULL.\n");
		return ;
	}
	while (strarr[i])
	{
		printf("[%i] -> %s\n", i, strarr[i]);
		i++;
	}
}

void	write_stderr(char *str)
{
	int	strlen;

	if (!str)
		str = "Error occured.\n";
	strlen = ft_strlen(str);
	write(2, str, strlen);
	write(2, "\n", 1);
}

void	*ft_realloc(void *oldptr, size_t oldsize, size_t newsize)
{
	void			*newptr;
	unsigned char	*bytenew;
	unsigned char	*byteold;
	size_t			i;

	i = 0;
	if (newsize == 0)
	{
		free(oldptr);
		return (NULL);
	}
	newptr = (void *)malloc(newsize);
	if (!newptr)
		return (NULL);
	if (!oldptr)
		return (newptr);
	byteold = (unsigned char *)oldptr;
	bytenew = (unsigned char *)newptr;
	while (i < newsize && i < oldsize)
	{
		bytenew[i] = byteold[i];
		i++;
	}
	free(oldptr);
	return (newptr);
}

char	*str_realloc(char *oldstr, size_t newsize)
{
	char	*newstr;
	size_t	oldsize;

	if (oldstr && oldstr[0] != '\0')
		oldsize = (size_t)ft_strlen(oldstr);
	else
		oldsize = 0;
	newstr = (char *)ft_realloc(oldstr, oldsize, newsize);
	if (!newstr || newsize < 1)
		return (NULL);
	newstr[newsize - 1] = '\0';
	return (newstr);
}

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	i;
	size_t	len;
	char	*copy_s1;

	i = 0;
	len = 0;
	if (!s1)
		return (write_stderr("s1 is NULL in ft_strndup.\n"), NULL);
	while (len < n && s1[len])
		len++;
	copy_s1 = (char *)malloc((len + 1) * sizeof(char));
	if (!copy_s1)
	{
		perror("Failed to allocate memory for s1 in ft_strndup function");
		exit(EXIT_FAILURE);
	}
	while (i < n && s1[i])
	{
		copy_s1[i] = s1[i];
		i++;
	}
	copy_s1[i] = '\0';
	return (copy_s1);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}
