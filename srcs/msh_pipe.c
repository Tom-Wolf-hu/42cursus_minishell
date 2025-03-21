/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:19:10 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/20 19:46:20 by tfarkas          ###   ########.fr       */
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

In this pipe_read function the redirection not correct

void	pipe_read(t_store *st)
{
	if (st->fd_exin > 2 && st->cmd_num == 1)
	{
		if (dup2(st->pipefd[st->cmd_num % 2][0], st->fd_exin) < 0)
		{
			perror("Failed to duplicate pipefd[0] to st->fd_exin.");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (dup2(st->pipefd[st->cmd_num % 2][0], STDIN_FILENO) < 0)
		{
			perror("Failed to duplicate pipefd[0] to stdin.");
			exit(EXIT_FAILURE);
		}
	}
	close(st->pipefd[st->cmd_num % 2][0]);
	if (st->fd_exin > 2)
		close(st->fd_exin);
}

*/

void	pipe_read(t_store *st)
{
	if (st->fd_exin > 0)
	{
		if (dup2(st->fd_exin, STDIN_FILENO) < 0)
		{
			perror("Failed to duplicate st->fd_exin to stdin.");
			exit(EXIT_FAILURE);
		}
		monitor_fds(st, FD_EXIN, DUPLICATE_FD);
		close(st->fd_exin);
		monitor_fds(st, FD_EXIN, CLOSE_FD);
	}
	if (st->cmd_num < st->pipecount)
	{
		close(st->pipefd[st->cmd_num % 2][0]);
		monitor_fds(st, PIPEFD_R, CLOSE_FD);
	}
	// st->fd_exin = st->pipefd[st->cmd_num % 2][0];
}

void	pipe_write(t_store *st)
{
	if (st->fd_exout > 2 && st->cmd_num == st->pipecount + 1)
	{
		if (dup2(st->fd_exout, STDOUT_FILENO) < 0)
		{
			perror("Failed to duplicate st->fd_exout to stdout.");
			exit(EXIT_FAILURE);
		}
		monitor_fds(st, FD_EXOUT, DUPLICATE_FD);
		close(st->fd_exout);
		monitor_fds(st, FD_EXOUT, CLOSE_FD);
	}
	else if (st->cmd_num < st->pipecount)
	{
		// printf("%d\n", st->pipefd[st->cmd_num % 2][1]);
		// fds_state();
		if (dup2(st->pipefd[st->cmd_num % 2][1], STDOUT_FILENO) < 0)
		{
			perror("Failed to duplicate pipefd[1] to stdout.");
			exit(EXIT_FAILURE);
		}
		monitor_fds(st, PIPEFD_W, DUPLICATE_FD);
		close(st->pipefd[st->cmd_num % 2][1]);
		monitor_fds(st, PIPEFD_W, CLOSE_FD);
	}
}

void	chproc_fd(t_store *st)
{
	if (st->pipecount > 0)
	{
		pipe_read(st);
		pipe_write(st);
	}
	close(st->save_stdin);
	monitor_fds(st, SAVE_STDIN, CLOSE_FD);
	close(st->save_stdout);
	monitor_fds(st, SAVE_STDOUT, CLOSE_FD);
	// close(st->fd_readl);
}

void	parent_fd(t_store *st)
{
	if (st->fd_exin > 0)
	{
		close(st->fd_exin);
		monitor_fds(st, FD_EXIN, CLOSE_FD);
	}
	close(st->pipefd[st->cmd_num % 2][1]);
	monitor_fds(st, PIPEFD_W, CLOSE_FD);
	st->fd_exin = st->pipefd[st->cmd_num % 2][0];
}

void	pipe_create(t_store *st)
{
	if (st->pipecount > 0 && st->cmd_num <= st->pipecount)
	{
		if (pipe(st->pipefd[st->cmd_num % 2]) < 0)
		{
			perror("Failed to create pipe");
			exit(EXIT_FAILURE);
		}
		monitor_fds(st, PIPEFD_W, OPEN_FD);
		monitor_fds(st, PIPEFD_R, OPEN_FD);
	}
}

void	gnl_readline(t_store *st, int *status)
{
	char	*line;

	line = get_next_line(st->fd_readl);
	while (line != NULL)
	{
		st->cmd_num++;
		pipe_create(st);
		// write(1, "passed1\n", 8);
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
	monitor_fds(st, FD_READL, OPEN_FD);
	while (line[i])
	{
		if (line[i] == '|')
		{
			// write(1, "pipe\n", 5);
			st->pipecount++;
			write(fd_readl, "\n", 1);
		}
		else
			write(fd_readl, &line[i], 1);
		i++;
	}
	close(fd_readl);
	monitor_fds(st, FD_READL, CLOSE_FD);
}

int	read_readline(t_store *st)
{
	int		status;

	st->fd_readl = open(".temp_readline", O_RDONLY | O_CLOEXEC);
	if (st->fd_readl < 0)
	{
		perror("Failed open fd_readl filedescriptor.");
		return (1);
	}
	monitor_fds(st, FD_READL, OPEN_FD);
	gnl_readline(st, &status);
	status = wait_child(st, status);
	write(1, "1passed1\n", 9);
	reset_fds(st);
	write(1, "1passed2\n", 9);
	// printf("after reset: \n");
	// fds_state();
	if (unlink(".temp_readline") < 0)
		perror("Failed to unlink the temp_readline temporary file");
	return (status);
}
