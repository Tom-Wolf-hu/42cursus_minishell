/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:08:11 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/17 19:28:43 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	red_in(char *fd_name)
{
	int	fd_redin;

	fd_redin = open(fd_name, O_RDONLY);
	if (fd_redin < 0)
	{
		perror("Failed to open file descriptor for input file.\n");
		return ;
	}
	if (dup2(fd_redin, STDIN_FILENO) < 0)
		perror("Failed to duplicate input file's file descriptor.\n");
	close(fd_redin);
}

void	red_out(char *fd_name)
{
	int	fd_redout;

	fd_redout = open(fd_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_redout < 0)
	{
		perror("Failed to open file descriptor for output file.\n");
		return ;
	}
	if (dup2(fd_redout, STDOUT_FILENO) < 0)
		perror("Failed to duplicate output file's file descriptor.\n");
	close(fd_redout);
}

void	red_out_append(char *fd_name)
{
	int	fd_redoutap;

	fd_redoutap = open(fd_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_redoutap < 0)
	{
		perror("Failed to open file descriptor for output file.\n");
		return ;
	}
	if (dup2(fd_redoutap, STDOUT_FILENO) < 0)
		perror("Failed to duplicate output file's file descriptor.\n");
	close(fd_redoutap);
}

void	read_in_temp(char *delimeter, int fd_delimeter)
{
	char	*read_l;

	rl_on_new_line();
	while (true)
	{
		read_l = readline(">");
		if (!read_l || ft_strcmp(read_l, delimeter) == 0)
			break ;
		ft_putendl_fd(read_l, fd_delimeter);
		free(read_l);
	}
	if (read_l)
		free(read_l);
}

void	red_del(char *delimeter)
{
	int	fd_delimeter;

	fd_delimeter = open(delimeter, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd_delimeter < 0)
	{
		perror("Failed to open file descriptor for temporary file.\n");
		return ;
	}
	read_in_temp(delimeter, fd_delimeter);
	if (unlink(delimeter) < 0)
		perror("Failed to unlink the temporary file.\n");
	if (dup2(fd_delimeter, STDIN_FILENO) < 0)
		perror("Failed to duplicate temporary file's file descriptor.\n");
	close(fd_delimeter);
}
