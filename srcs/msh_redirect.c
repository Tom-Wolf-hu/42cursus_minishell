/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:08:11 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/25 18:37:38 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	red_in(char *fd_name)
{
	int	fd_redin;

	fd_redin = open(fd_name, O_RDONLY);
	if (fd_redin < 0)
	{
		perror("Failed to open file descriptor for input file");
		return (-1);
	}
	// if (dup2(fd_redin, STDIN_FILENO) < 0)
	// 	perror("Failed to duplicate input file's file descriptor");
	return (fd_redin);
}

int	red_out(char *fd_name)
{
	int	fd_redout;

	fd_redout = open(fd_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_redout < 0)
	{
		perror("Failed to open file descriptor for output file");
		exit(EXIT_FAILURE);
	}
	// if (dup2(fd_redout, STDOUT_FILENO) < 0)
	// {
	// 	perror("Failed to duplicate output file's file descriptor");
	// 	exit(EXIT_FAILURE);
	// }
	return (fd_redout);
}

int	red_out_append(char *fd_name)
{
	int	fd_redoutap;

	fd_redoutap = open(fd_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_redoutap < 0)
	{
		perror("Failed to open file descriptor for output file");
		exit(EXIT_FAILURE);
	}
	// if (dup2(fd_redoutap, STDOUT_FILENO) < 0)
	// {
	// 	perror("Failed to duplicate output file's file descriptor");
	// 	exit(EXIT_FAILURE);
	// }
	return (fd_redoutap);
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

int	red_del(char *delimeter)
{
	int	fd_delimeter;
	char *tmp_file;

	tmp_file = ".heredoc_file";
	fd_delimeter = open(tmp_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_delimeter < 0)
	{
		perror("Failed to open file descriptor for temporary file");
		return (-1);
	}
	read_in_temp(delimeter, fd_delimeter);
	close(fd_delimeter);
	fd_delimeter = open(tmp_file, O_RDONLY);
	if (fd_delimeter < 0)
		perror("Failed to reopen file descriptor for tmp_file");
	// if (dup2(fd_delimeter, STDIN_FILENO) < 0)
	// 	perror("Failed to duplicate temporary file's file descriptor");
	if (unlink(tmp_file) < 0)
		perror("Failed to unlink the heredoc temporary file");
	return (fd_delimeter);
}
