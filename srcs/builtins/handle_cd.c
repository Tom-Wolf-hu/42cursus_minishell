/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:26 by alex              #+#    #+#             */
/*   Updated: 2025/03/17 16:10:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_cd(char *line)
{
	char	**arr;
	char	*path;
	int 	i;

	i = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1), 1);
	while (arr[i])
		i++;
	if (i > 2)
	{
		printf("minishell: cd: too many arguments\n");
		free_arr(arr);
		return (1);
	}
	if (i == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			free_arr(arr);
			return (1);
		}
	}
	else
		path = arr[1];
	if (chdir(path) != 0)
		return (perror("minishell: cd"), 1);
	free_arr(arr);
	return (0);
}
