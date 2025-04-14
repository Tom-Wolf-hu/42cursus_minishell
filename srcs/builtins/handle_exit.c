/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:32 by alex              #+#    #+#             */
/*   Updated: 2025/04/14 14:48:19 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_exit_args(char **line_arr, int *status)
{
	int	count;

	count = 0;
	while (line_arr[count] != NULL)
		count++;
	if (count > 2)
	{
		printf("minishell: exit: too many argument\n");
		*status = 1;
		return (1);
	}
	else if (line_arr[1])
	{
		// line_arr[1] = remove_quotes_first_word(line_arr[1]);
		if (!is_nummeric(line_arr[1]))
		{
			printf("minishell: exit: %s: numeric "
				"argument required\n", line_arr[1]);
			*status = 255;
			return (0);
			// return (free_arr(line_arr), exit(*status));
		}
		*status = ft_atoi(line_arr[1]);
	}
	return (0);
}

void	handle_exit(char *line, int *status)
{
	char	**line_arr;

	if (!line)
		return ;
	line_arr = ft_split(line, ' ');
	if (check_exit_args(line_arr, status) == 1)
		return ;
	free_arr(line_arr);
	free(line);
	rl_clear_history();
	free_var_after_exit();
	exit(*status);
}

// void	handle_exit(char *line, int *status)
// {
// 	char	**line_arr;

// 	if (!line)
// 		return ;
// 	line_arr = ft_split(line, ' ');
// 	if (line_arr[1])
// 	{
// 		if (!is_nummeric(line_arr[1]))
// 		{
// 			printf("minishell: exit: %s: numeric "
// 				"argument required\n", line_arr[1]);
// 			*status = 255;
// 			// return (free_arr(line_arr), exit(*status));
// 			return (free_arr(line_arr));
// 		}
// 		*status = ft_atoi(line_arr[1]);
// 	}
// 	free_arr(line_arr);
// 	free(line);
// 	rl_clear_history();
// 	free_var_after_exit();
// 	exit(*status);
// }
