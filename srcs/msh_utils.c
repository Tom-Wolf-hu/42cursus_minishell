/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:44:21 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/18 21:12:28 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_stderr(char *str)
{
	int	strlen;

	if (!str)
		str = "Error occured.\n";
	strlen = ft_strlen(str);
	write(2, str, strlen);
	write(2, "\n", 1);
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
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
