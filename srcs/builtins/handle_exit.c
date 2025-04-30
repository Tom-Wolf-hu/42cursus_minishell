/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:32 by alex              #+#    #+#             */
/*   Updated: 2025/04/30 13:47:28 by omalovic         ###   ########.fr       */
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

void	free_error(char **line_arr, struct s_saved_std *std, int *status)
{
	write_stderr("minishell: exit: too many arguments");
	*status = 1;
	free_arr(line_arr);
	if (std)
		close_saved_std(std);
}

void	handle_exit(char *line, int *status,
			struct s_saved_std *std, char **myenvp)
{
	char	**line_arr;
	char	*clean_arg;
	int		arr_len;

	if (!line)
		return ;
	line_arr = ft_split(line, ' ');
	arr_len = ft_arrlen(line_arr);
	if (arr_len == 2)
	{
		clean_arg = remove_quotes(line_arr[1]);
		if (!is_nummeric(clean_arg))
		{
			printf("minishell: exit: %s: numeric argument required\n",
				clean_arg);
			*status = 255;
			return (free_all(line_arr, line, myenvp, std),
				free(clean_arg), exit(*status));
		}
		*status = ft_atoi(clean_arg);
	}
	else if (arr_len > 2)
		return (free_error(line_arr, std, status));
	return (free_all(line_arr, line, myenvp, std), exit(*status));
}
