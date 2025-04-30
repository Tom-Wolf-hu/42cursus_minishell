/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:22:04 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/30 17:15:55 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	in_redir(char *filename, int *status)
{
	int	file_fd;

	file_fd = open(filename, O_RDONLY);
	if (file_fd == -1)
	{
		perror("open");
		*status = 1;
		return (1);
	}
	dup2(file_fd, STDIN_FILENO);
	close(file_fd);
	return (0);
}

int	out_redir(char *filename, int *status, int *i, char opt)
{
	int	file_fd;

	if (opt == 'a')
	{
		file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		(*i)++;
	}
	else
		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_fd == -1)
	{
		perror("open");
		*status = 1;
		return (1);
	}
	dup2(file_fd, STDOUT_FILENO);
	close(file_fd);
	return (0);
}

void	reset_stdin(void)
{
	int	std;

	if (!isatty(STDIN_FILENO))
	{
		std = open("/dev/tty", O_RDWR);
		if (std == -1)
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
