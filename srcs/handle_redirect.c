/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:25:23 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/29 17:24:32 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	heredoc_parent(char *filename, int *status, int pipe_fd[2], pid_t pid)
{
	int	wstatus;

	close(pipe_fd[1]);
	waitpid(pid, status, 0);
	if (WIFSIGNALED(*status))
	{
		write(STDOUT_FILENO, "\n", 1);
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		return (*status = 130, 1);
	}
	else if (WIFEXITED(*status))
	{
		wstatus = WEXITSTATUS(*status);
		if (wstatus != 0)
			return (*status = wstatus, 1);
	}
	signal(SIGINT, sig_handler);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	return (0);
}

int	heredoc_pipe_sign(char *filename, int *status, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) < 0)
	{
		perror("Failed to create pipe in handle redirection");
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("Failed to create for for heredoc");
		return (1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(pipe_fd[0]);
		handle_heredoc_child(pipe_fd[1], filename, status, envp);
	}
	else
		return (heredoc_parent(filename, status, pipe_fd, pid));
	return (0);
}

int	ch_redirect(t_redirect_args *args, int *i, int *status, char **envp)
{
	if (args->line[*i] == '<' && args->line[*i + 1] == '<')
	{
		if (heredoc_pipe_sign(args->filename, status, envp) == 1)
			return (1);
		*i += 2;
	}
	else if (args->line[*i] == '>' && args->line[*i + 1] == '>')
	{
		if (out_redir(args->filename, status, i, 'a') == 1)
			return (1);
	}
	else if (args->line[*i] == '>')
	{
		if (out_redir(args->filename, status, i, 't') == 1)
			return (1);
	}
	else if (args->line[*i] == '<')
	{
		if (in_redir(args->filename, status) == 1)
			return (1);
	}
	return (0);
}

int	handle_redirection(char *line, int *status, char **envp)
{
	struct s_redirect_args	args;
	int						i;

	i = 0;
	args.line = line;
	while (args.line[i])
	{
		if (args.line[i] == '<' || args.line[i] == '>')
		{
			args.filename = get_filename(args.line + i);
			if (!args.filename)
			{
				write_stderr("No such file or directory");
				*status = 1;
				return (1);
			}
			if (ch_redirect(&args, &i, status, envp) == 1)
				return (free(args.filename), 1);
			free(args.filename);
		}
		i++;
	}
	return (0);
}
