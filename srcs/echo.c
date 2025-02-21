/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:40:45 by omalovic          #+#    #+#             */
/*   Updated: 2025/02/21 16:00:23 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	mywrite(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	if (line[i] == 34)
		i++;
	if (line[len - 1] == 34)
		len--;
	while (i < len)
	{
		write(1, &line[i], 1);
		i++;
	}
}

void	show_input(char **arr, int flag)
{
	int	i;
	int len;

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
			mywrite(arr[i]);
			if (!flag)
				write(1, "\n", 1);
			return ;
		}
		mywrite(arr[i]);
		write(1, " ", 1);
		i++;
	}
}

int	check_quastion_sign(char *line)
{
	
}

void	handle_echo(char *line)
{
	char	**arr;
	int		i;
	char	*line_to_check;

	line_to_check = ft_strdup(line);
	if (!check_quastion_sign(line))
	{
		bridge_var(&line_to_check);
	}
	i = 0;
	arr = ft_split(line_to_check, ' ');
	if (!arr)
		return (free(line), free(line_to_check), exit(1));
	while (arr[i])
		i++;
	if (i == 1)
	{
		return (write(1, "\n", 1), free_arr(arr), free(line_to_check));
	}
	if (i == 2)
	{
		if (ft_strcmp(arr[1], "-n") == 0)
			return (free_arr(arr), free(line), free(line_to_check));
		else
			return (mywrite(arr[1]), write(1, "\n", 1), (void)0);
	}
	if (i > 2)
	{
		if (ft_strcmp(arr[1], "-n") == 0)
			return (show_input(arr, 1), free_arr(arr));
		show_input(arr, 0);
	}
	free_arr(arr);
	// free(line);
	// free(line_to_check);
}
