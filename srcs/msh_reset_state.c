/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_reset_state.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:55:15 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/26 13:17:15 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
Later it needs to check if it has bad effect on script 
running or not.
*/
void	set_back_stds(void)
{
	int	tty;

	tty = open("/dev/tty", O_RDWR);
	if (tty < 0)
	{
		perror("Failed to open tty for stds");
		exit(EXIT_FAILURE);
	}
	if (!isatty(STDIN_FILENO))
	{
		if (dup2(tty, STDIN_FILENO) < 0)
		{
			perror("Failed to reset tty stdin");
			exit(EXIT_FAILURE);
		}
	}
	if (!isatty(STDOUT_FILENO))
	{
		if (dup2(tty, STDOUT_FILENO) < 0)
		{
			perror("Failed to reset tty stdout");
			exit(EXIT_FAILURE);
		}
	}
	close(tty);
}
