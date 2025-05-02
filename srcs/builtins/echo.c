/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:40:45 by omalovic          #+#    #+#             */
/*   Updated: 2025/05/02 12:22:42 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	wr_stillquotes(char *line, int fd, int *i, char quotes)
{
	int	len;

	len = ft_strlen(line);
	(*i)++;
	while (*i < len)
	{
		if (line[*i] == '\\' && quotes == '\"' && *i + 1 < len)
		{
			(*i)++;
		}
		else if (line[*i] == quotes)
			break ;
		write(fd, &line[*i], 1);
		(*i)++;
	}
	if (*i < len && line[*i] == quotes)
		(*i)++;
}

void	mywrite(char *line, int fd)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(line);
	while (i < len)
	{
		if (line[i] == '\'' || line[i] == '\"')
			wr_stillquotes(line, fd, &i, line[i]);
		else
		{
			write(fd, &line[i], 1);
			i++;
		}
	}
}

void	show_input(char **arr, int fd, int flag)
{
	int	i;
	int	len;

	if (flag)
		i = 2;
	else
		i = 1;
	len = 0;
	while (arr[len])
		len++;
	while (arr[i])
	{
		if (i == len - 1)
		{
			mywrite(arr[i], fd);
			if (!flag)
				write(fd, "\n", 1);
			return ;
		}
		mywrite(arr[i], fd);
		write(fd, " ", 1);
		i++;
	}
}

int	only_contains_char(char *str, char ch)
{
	while (*str)
	{
		if (*str != ch)
			return (0);
		str++;
	}
	return (1);
}

int	handle_echo(char *line, int fd)
{
	char	**arr;
	int		i;

	i = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1), 1);
	while (arr[i])
		i++;
	if (i == 1)
		return (write(fd, "\n", 1), free_arr(arr), 0);
	if (i == 2)
	{
		if (ft_strcmp(arr[1], "-n") == 0)
			return (free_arr(arr), 0);
		return (mywrite(arr[1], fd), write(1, "\n", 1), free_arr(arr), 0);
	}
	if (i > 2)
	{
		if (ft_strncmp(arr[1], "-n", 2) == 0
			&& only_contains_char(arr[1] + 1, 'n'))
			return (show_input(arr, fd, 1), free_arr(arr), 0);
		show_input(arr, fd, 0);
	}
	return (free_arr(arr), 0);
}
