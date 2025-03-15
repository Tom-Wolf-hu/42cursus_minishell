/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:19:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/15 19:27:49 by tfarkas          ###   ########.fr       */
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


void	ft_pipe(t_store *st)
{
	int	pipefd[2];

	(void)st;
	// check_tty();
	// printf("%i\n", st->save_stdin);
	// printf("%i\n", st->save_stdout);
	// printf("%i\n", STDIN_FILENO);
	// printf("%i\n", STDOUT_FILENO);
	write(1, "passed1\n", 8);
	if (pipe(pipefd) < 0)
	{
		perror("Pipe creation failed.\n");
		return ;
	}
	// fds_state();
	write(1, "passed2\n", 8);
	if (dup2(pipefd[1], STDOUT_FILENO) < 0)
	{
		perror("Failed to duplicate pipefd[1]");
		close(pipefd[0]);
		close(pipefd[1]);
		return ;
	}
	write(1, "passed3\n", 8);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
	{
		perror("Failed to duplicate pipefd[0]");
		// close(pipefd[0]);
		// close(pipefd[1]);
		// return ;
	}
	fds_state();
	write(1, "passed4\n", 8);
	close(pipefd[0]);
	close(pipefd[1]);
	write(1, "passed5\n", 8);
	check_tty();
	printf("%i\n", st->save_stdin);
	printf("%i\n", STDIN_FILENO);
}


This option for pipe reuse idea. However very difficult to connect with
the existing functions. This reason I try the filedescriptor managger idea.

void	ft_pifork(t_store *st, int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Failed to fork.");
		return ;
	}
	if (pid == 0)
	{
		if (st->fd_exin != 0)
		{
			dup2(st->fd_exin, STDIN_FILENO);
		}
	}
	else
		
}

void	ft_pipe(t_store *st)
{
	int		pipefd[2][2];
	int		i;

	i = 0;
	if (st->pidcount < 1)
		return ;
	while (i < st->pidcount)
	{
		if (pipe(pipefd[i % 2]) < 0)
		{
			perror("Pipe creation failed.\n");
			return ;
		}
		ft_pifork(st, pipefd[i % 2]);
		i++;
	}
}
*/

void	pipe_read(t_store *st)
{
	if (st->fd_exin > 2 && st->cmd_num == 1)
	{
		if (dup2(st->pipefd[0], st->fd_exin) < 0)
		{
			perror("Failed to duplicate pipefd[0] to st->fd_exin.");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (dup2(st->pipefd[0], STDIN_FILENO) < 0)
		{
			perror("Failed to duplicate pipefd[0] to stdin.");
			exit(EXIT_FAILURE);
		}
	}
	close(st->pipefd[0]);
}

void	pipe_write(t_store *st)
{
	if (st->fd_exout > 2 && st->cmd_num == st->pipecount + 1)
	{
		if (dup2(st->pipefd[1], st->fd_exout) < 0)
		{
			perror("Failed to duplicate pipefd[0] to st->fd_exout.");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (dup2(st->pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("Failed to duplicate pipefd[0] to stdout.");
			exit(EXIT_FAILURE);
		}
	}
	close(st->pipefd[1]);
}

void	chproc_fd(t_store *st)
{
	if (st->pipecount > 0)
	{
		pipe_read(st);
		pipe_write(st);
	}
	close(st->save_stdin);
	close(st->save_stdout);
	close(st->fd_readl);
}

void	gnl_readline(t_store *st, int *status)
{
	char	*line;

	line = get_next_line(st->fd_readl);
	while (line != NULL)
	{
		st->cmd_num++;
		if (st->pipecount > 0)
		{
			if (pipe(st->pipefd) < 0)
			{
				perror("Failed to create pipe");
				exit(EXIT_FAILURE);
			}
		}
		*status = redir_cmd_s(line, st);
		free(line);
		line = get_next_line(st->fd_readl);
	}
}

void	temp_readline(char *line, t_store *st)
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
		{
			st->pipecount++;
			write(fd_readl, "\n", 1);
		}
		else
			write(fd_readl, &line[i], 1);
		i++;
	}
	close(fd_readl);
}

int	read_readline(t_store *st)
{
	char	*line;
	int		status;

	st->fd_readl = open(".temp_readline", O_RDONLY);
	if (st->fd_readl < 0)
	{
		perror("Failed open fd_readl filedescriptor.");
		return (1);
	}
	gnl_readline(st, &status);
	status = wait_child(st, status);
	reset_fds(st);
	if (unlink(".temp_readline") < 0)
		perror("Failed to unlink the temp_readline temporary file");
	return (status);
}
