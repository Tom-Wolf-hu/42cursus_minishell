/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:35:43 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/15 16:41:20 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_stdin(void)
{
	int	std;

	if (!isatty(STDIN_FILENO))
	{
		std = open("/dev/tty", O_RDWR);
		if (!std)
		{
			perror("Failed restore STDIN");
			exit(EXIT_FAILURE);
		}
		if (dup2(std, STDIN_FILENO) < 0)
		{
			perror("Failed to duplicate STDIN");
			exit(EXIT_FAILURE);
		}
		close(std);
	}
}
