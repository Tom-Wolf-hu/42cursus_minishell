/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:32 by alex              #+#    #+#             */
/*   Updated: 2025/04/23 19:45:09 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_all(char **line_arr, char *line,
			char **myenvp, struct s_saved_std *std)
{
	free_arr(line_arr);
	free(line);
	rl_clear_history();
	free_arr(myenvp);
	printf("exit\n");
	if (std)
		close_saved_std(std);
}

void	handle_exit(char *line, int *status,
			struct s_saved_std *std, char **myenvp)
{
	char	**line_arr;

	if (!line)
		return ;
	line_arr = ft_split(line, ' ');
	if (line_arr[1])
	{
		if (!is_nummeric(line_arr[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n",
				line_arr[1]);
			*status = 255;
			return (free_all(line_arr, line, myenvp, std), exit(*status));
		}
		*status = ft_atoi(line_arr[1]);
	}
	free_all(line_arr, line, myenvp, std);
	exit(*status);
}
