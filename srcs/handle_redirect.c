/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:32:51 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/15 20:20:14 by tfarkas          ###   ########.fr       */
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
		free(filename);
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
		free(filename);
		return (1);
	}
	dup2(file_fd, STDOUT_FILENO);
	close(file_fd);
	return (0);
}

void	handle_redirection(char *line, int *status)
{
	int		i = 0;
	int		file_fd;
	char	*filename;
	struct s_saved_std std;

	*status = 0;
	// printf("[handle_redirection] status: %d\n", *status);
	while (line[i])
	{
		// printf("[handle_redirection] in cycle...\n");
		if (line[i] == '<' || line[i] == '>')
		{
			filename = get_filename(line + i);
			// printf("filename: %s\n", filename);
			if (!filename)
			{
				printf("Such name for file doesn't exist\n");
				*status = 1;
				return ;
			}
			if (line[i] == '<' && line[i + 1] == '<') // heredoc
			{
				int	pipe_fd[2];
				pid_t pid;

				if (pipe(pipe_fd) < 0)
				{
					perror("Failed to create pipe in handle redirection");
					return ;
				}
				signal(SIGINT, SIG_IGN);
				pid = fork();
				if (pid == -1)
				{
					perror("Failed to create for for heredoc");
					return;
				}
				if (pid == 0)
				{
					signal(SIGINT, SIG_DFL);
					close(pipe_fd[0]);
					handle_heredoc_child(pipe_fd[1], filename);
					// handle_heredoc(filename);
					// exit(0);
				}
				else
				{
					close(pipe_fd[1]);
					waitpid(pid, status, 0);
					if (WIFSIGNALED(*status))
					{
						write(STDOUT_FILENO, "\n", 1);
						// write(STDOUT_FILENO, "tamas\n", 6);
						close(pipe_fd[1]);
						dup2(pipe_fd[0], STDIN_FILENO);
						close(pipe_fd[0]);
						g_status = 130;
						return ;
					}
					else if (WIFEXITED(*status))
					{
						int wstatus;
						wstatus = WEXITSTATUS(*status);
						if (wstatus != 0)
						{
							g_status = wstatus;
							return ;
						}
					}
					signal(SIGINT, sig_handler);
					dup2(pipe_fd[0], STDIN_FILENO);
					close(pipe_fd[0]);
					// return ;
				}
				i += 2;
			}
			else if (line[i] == '>' && line[i + 1] == '>')
			{
				if (out_redir(filename, status, &i, 'a') == 1)
					return ;
			}
			else if (line[i] == '>')
			{
				if (out_redir(filename, status, &i, 't') == 1)
					return ;
			}
			else if (line[i] == '<')
			{
				if (in_redir(filename, status) == 1)
					return;
			}
			free(filename);
		}
		i++;
	}
}

// void	handle_redirection(char *line, int *status)
// {
// 	int		i = 0;
// 	int		file_fd;
// 	char	*filename;
// 	struct s_saved_std std;

// 	*status = 0;
// 	// printf("[handle_redirection] status: %d\n", *status);
// 	while (line[i])
// 	{
// 		// printf("[handle_redirection] in cycle...\n");
// 		if (line[i] == '<' || line[i] == '>')
// 		{
// 			filename = get_filename(line + i);
// 			// printf("filename: %s\n", filename);
// 			if (!filename)
// 			{
// 				printf("Such name for file doesn't exist\n");
// 				*status = 1;
// 				return ;
// 			}
// 			if (line[i] == '<' && line[i + 1] == '<') // heredoc
// 			{
// 				int	pipe_fd[2];
// 				pid_t pid;

// 				if (pipe(pipe_fd) < 0)
// 				{
// 					perror("Failed to create pipe in handle redirection");
// 					return ;
// 				}
// 				signal(SIGINT, SIG_IGN);
// 				pid = fork();
// 				if (pid == -1)
// 				{
// 					perror("Failed to create for for heredoc");
// 					return;
// 				}
// 				if (pid == 0)
// 				{
// 					signal(SIGINT, SIG_DFL);
// 					close(pipe_fd[0]);
// 					handle_heredoc_child(pipe_fd[1], filename);
// 					// handle_heredoc(filename);
// 					// exit(0);
// 				}
// 				else
// 				{
// 					close(pipe_fd[1]);
// 					waitpid(pid, status, 0);
// 					if (WIFSIGNALED(*status))
// 					{
// 						write(STDOUT_FILENO, "\n", 1);
// 						// write(STDOUT_FILENO, "tamas\n", 6);
// 						close(pipe_fd[1]);
// 						dup2(pipe_fd[0], STDIN_FILENO);
// 						close(pipe_fd[0]);
// 						g_status = 130;
// 						return ;
// 					}
// 					else if (WIFEXITED(*status))
// 					{
// 						int wstatus;
// 						wstatus = WEXITSTATUS(*status);
// 						if (wstatus != 0)
// 						{
// 							g_status = wstatus;
// 							return ;
// 						}
// 					}
// 					signal(SIGINT, sig_handler);
// 					dup2(pipe_fd[0], STDIN_FILENO);
// 					close(pipe_fd[0]);
// 					// return ;
// 				}
// 				i += 2;
// 			}
// 			else if (line[i] == '>' && line[i + 1] == '>') // append >>
// 			{
// 				// printf("sign: >>\n");
// 				file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 				if (file_fd == -1)
// 				{
// 					perror("open");
// 					*status = 1;
// 					break ;
// 				}
// 				dup2(file_fd, STDOUT_FILENO);
// 				close(file_fd);
// 				i++;
// 			}
// 			else if (line[i] == '>') // truncate >
// 			{
// 				// printf("sign: >\n");
// 				file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 				if (file_fd == -1)
// 				{
// 					perror("open");
// 					*status = 1;
// 					break ;
// 				}
// 				dup2(file_fd, STDOUT_FILENO);
// 				close(file_fd);
// 			}
// 			else if (line[i] == '<') // input <
// 			{
// 				// printf("sign: <\n");
// 				file_fd = open(filename, O_RDONLY);
// 				if (file_fd == -1)
// 				{
// 					perror("open");
// 					*status = 1;
// 					break ;
// 				}
// 				dup2(file_fd, STDIN_FILENO);
// 				close(file_fd);
// 			}
// 			free(filename);
// 		}
// 		i++;
// 	}
// }