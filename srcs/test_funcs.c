/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 16:35:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/10 12:40:22 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fds_state(void)
{
	int	fd_i;

	fd_i = 0;
	while (fd_i < 15)
	{
		if (fcntl(fd_i, F_GETFD) != -1)
			printf("\033[1;32mThe [%d] fd is open.\033[0m\n", fd_i);
		else
			printf("\033[1;31mThe [%d] fd is closed.\033[0m\n", fd_i);
		fd_i++;
	}
}

void check_tty()
{
	if (isatty(STDIN_FILENO))
		printf("\033[1;33mSTDIN is a terminal.\033[0m\n");
	else
		printf("\033[1;31mSTDIN NOT a terminal.\033[0m\n");
	if (isatty(STDOUT_FILENO))
		printf("\033[1;32mSTDOUT is a terminal.\033[0m\n");
	else
		printf("\033[1;31mSTDOUT NOT a terminal.\033[0m\n");
}