/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:32 by alex              #+#    #+#             */
/*   Updated: 2025/04/10 15:04:33 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_exit(char *line, int *status)
{
	char **line_arr;

	if (!line)
		return ;
	line_arr = ft_split(line, ' ');
	if (line_arr[1])
	{
		if (!is_nummeric(line_arr[1]))
		{
			printf("minishell: exit: %s: numeric argument required\n", line_arr[1]);
			*status = 255;
			return (free_arr(line_arr), exit(*status));
		}
		*status = ft_atoi(line_arr[1]);
	}
	free_arr(line_arr);
	free(line);
	rl_clear_history();
	free_var_after_exit();
	exit(*status);
}
