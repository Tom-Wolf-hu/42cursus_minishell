/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:19:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/10 20:25:48 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
The pipe part still didn't tested. I wrote the t_store st 
just that case we can compile the program. Later I need to go deep how we wnat to solve it.

void	pipe_dup(int pipefd[2], int which, char *beforep, char *afterp)
{
	t_store	*st;

	st = NULL;
	if (which == 1)
	{
		if (dup2(pipefd[0], STDOUT_FILENO) < 0)
			return ;
		close(pipefd[0]);
		choose_cmd(beforep, st);
	}
	else if (which == 2)
	{
		if (dup2(pipefd[1], STDIN_FILENO) < 0)
			return ;
		close(pipefd[1]);
		choose_cmd(afterp, st);
	}
}

void	ft_pipe(char *beforep, char *afterp)
{
	int	pipefd[2];

	if (pipe(pipefd) < 0)
	{
		perror("Pipe creation failed.\n");
		return ;
	}
	if (beforep)
	{
		close(pipefd[1]);
		pipe_dup(pipefd, 1, beforep, afterp);
	}
	if (afterp)
	{
		close(pipefd[0]);
		pipe_dup(pipefd, 2, beforep, afterp);
	}
	close(pipefd[0]);
	close(pipefd[1]);
}

I created a simple pipe function in the next ft_pipe version, where
the function will just create the pipe. What we will need test later if
is enough if we redericted just the stdin and stdout, or we need to pass
the redirected file descriptors to the pipe function.

*/

void	ft_pipe(t_store *st)
{
	int	pipefd[2];

	(void)st;
	// fds_state();
	// check_tty();
	// printf("%i\n", st->save_stdin);
	// printf("%i\n", STDIN_FILENO);
	write(1, "passed1\n", 8);
	if (pipe(pipefd) < 0)
	{
		perror("Pipe creation failed.\n");
		return ;
	}
	write(1, "passed2\n", 8);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		perror("Failed to duplicate pipefd[0]");
		close(pipefd[0]);
		close(pipefd[1]);
		return ;
	}
	write(1, "passed3\n", 8);
	if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		perror("Failed to duplicate pipefd[1]");
	fds_state();
	write(1, "passed4\n", 8);
	close(pipefd[0]);
	close(pipefd[1]);
	write(1, "passed5\n", 8);
	check_tty();
	printf("%i\n", st->save_stdin);
	printf("%i\n", STDIN_FILENO);
}

void temp_readline(char *line)
{
	int	fd_readl;
	int	i;

	i = 0;
	fd_readl = open(".temp_readline", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_readl < 0)
	{
		perror("Failed open fd_readl filedescriptor.");
		return ;
	}
	while (line[i])
	{
		if (line[i] == '|')
			write(fd_readl, "\n", 1);
		else
			write(fd_readl, &line[i], 1);
		i++;
	}
	close(fd_readl);
}

int	read_readline(t_store *st)
{
	int		fd_readl;
	char	*line;
	int		status;

	fd_readl = open(".temp_readline", O_RDONLY);
	if (fd_readl < 0)
	{
		perror("Failed open fd_readl filedescriptor.");
		return (1);
	}
	line = get_next_line(fd_readl);
	while (line != NULL)
	{
		// write(1, "1passed1\n", 9);
		status = redir_cmd_s(line, st);
		free(line);
		line = get_next_line(fd_readl);
		if (line != NULL)
		{
			write(1, "1passed2\n", 9);
			ft_pipe(st);
		}
	}
	// write(1, "1passed3\n", 9);
	status = wait_child(st);
	reset_fds(st);
	close(fd_readl);
	if (unlink(".temp_readline") < 0)
		perror("Failed to unlink the temp_readline temporary file");
	return (status);
}
