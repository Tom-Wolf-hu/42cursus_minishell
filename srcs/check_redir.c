/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:31:09 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/15 17:10:14 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_filename(char *cmd)
{
	int		i = 0, j = 0, start;
	char	*clean_filename;
	char	*filename;

	filename = NULL;
	while (cmd[i] && (cmd[i] == '>' || cmd[i] == '<'))
		i++;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n'))
		i++;
	if (!cmd[i])
		return (NULL);
	start = i;
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n' && cmd[i] != '<' && cmd[i] != '>')
		i++;
	filename = malloc(i - start + 1);
	if (!filename)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (start < i)
		filename[j++] = cmd[start++];
	filename[j] = '\0';
	clean_filename = remove_quotes_first_word(filename);
	return (clean_filename);
}

void	sig_handler_heredoc(int sig)
{
	if (sig == SIGINT)
		exit(130);
}

void	setup_signal_heredoc(void)
{
	struct sigaction sa;

	sa.sa_handler = sig_handler_heredoc;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void	handle_heredoc(const char *delimiter, int pipe_fd[2])
{
	char *line = NULL;

	setup_signal_heredoc();
	close(pipe_fd[0]);
	while (1)
	{
		// write(STDOUT_FILENO, "> ", 2);
		line = readline("heredoc> "); // НУЖНО ЗАПОМИНАТЬ LINE, ЧТОБЫ ЕГО ПОТОМ ВЫВЕСТИ
		if (!line || (ft_strncmp(line, delimiter, ft_strlen((char *)delimiter)) == 0 && ft_strlen(line) == ft_strlen((char *)delimiter)))
		{
			// write(STDERR_FILENO, "\n", 1);
			free(line);
			break ;
		}
		// printf("line: %s; len: %d\n", line, ft_strlen(line));
			// добавить еще условие на && line[strlen(delimiter)] == '\n'
		write(pipe_fd[1], line, strlen(line)); // Пишем в пайп
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
	// dup2(pipe_fd[0], STDIN_FILENO); // Перенаправляем stdin на пайп
	// close(pipe_fd[0]);
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
			printf("filename: %s\n", filename);
			if (!filename)
			{
				printf("Such name for file doesn't exist\n");
				*status = 1;
				return ;
			}
			if (line[i] == '<' && line[i + 1] == '<') // heredoc
			{
				int pipe_fd[2];
				pipe(pipe_fd);

				pid_t pid = fork();
				if (pid == 0)
				{
					handle_heredoc(filename, pipe_fd);
					exit(EXIT_SUCCESS);
				}
				else
				{
					int wstatus;
					close(pipe_fd[1]);
					waitpid(pid, &wstatus, 0);
					if (WIFEXITED(*status) && WEXITSTATUS(*status) == 130)
					{
						*status = 130;
						free(filename);
						return ;
					}
					dup2(pipe_fd[0], STDIN_FILENO);
					close(pipe_fd[0]);
				}
				// else
				// {
				// 	perror("fork");
				// 	exit(1);
				// }
				// i++;
				i+=2;
			}
			else if (line[i] == '>' && line[i + 1] == '>') // append >>
			{
				// printf("sign: >>\n");
				file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (file_fd == -1)
				{
					perror("open");
					*status = 1;
					break ;
				}
				dup2(file_fd, STDOUT_FILENO);
				close(file_fd);
				// i++;
				i+=2;
			}
			else if (line[i] == '>') // truncate >
			{
				// printf("sign: >\n");
				file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (file_fd == -1)
				{
					perror("open");
					*status = 1;
					break ;
				}
				dup2(file_fd, STDOUT_FILENO);
				close(file_fd);
			}
			else if (line[i] == '<') // input <
			{
				// printf("sign: <\n");
				file_fd = open(filename, O_RDONLY);
				if (file_fd == -1)
				{
					perror("open");
					*status = 1;
					break ;
				}
				dup2(file_fd, STDIN_FILENO);
				close(file_fd);
			}
			free(filename);
		}
		i++;
	}
}

void	join_part(char **s1, char *s2)
{
	int		lens1;
	int		lens2;
	char	*temp;
	int		i;

	lens1 = 0;
	i = 0;
	// fprintf(stderr, "s1: '%s'\ts2: '%s'\n", *s1, s2);
	if (*s1 != NULL)
		lens1 = ft_strlen(*s1);
	if (s2 == NULL)
		return ;
	lens2 = ft_strlen(s2);
	// fprintf(stderr, "lens1: %d\tlens2: %d\n", lens1, lens2);
	temp = (char *)malloc((lens1 + lens2 + 1) * sizeof(char));
	if (!temp)
	{
		perror("Failed to allocate memory for temp in join part");
		exit(EXIT_FAILURE);
	}
	while (*s1 && i < lens1)
	{
		temp[i] = (*s1)[i];
		// fprintf(stderr, "temp[%i] in while cycle: '%c'\n", i, temp[i]);
		i++;
	}
	// write(1, "1passed1\n", 9);
	// fprintf(stderr, "temp after s1 copy: %s\n", temp);
	i = 0;
	while (s2 && i < lens2)
	{
		temp[lens1 + i] = s2[i];
		i++;
	}
	// write(1, "1passed2\n", 9);
	// fprintf(stderr, "temp after s2 copy: %s\n", temp);
	// fprintf(stderr, "lens1 + i: %d\t lens1 + lens2 + 1: %d\n", lens1 + i, lens1 + lens2 + 1);
	temp[lens1 + i] = '\0';
	free(*s1);
	free(s2);
	s2 = NULL;
	*s1 = temp;
	// fprintf(stderr, "s1 in join_part: %s\n", *s1);
}

void	redir_part(char *cmd, int *i)
{
	while (cmd[*i] && (cmd[*i] == '<' || cmd[*i] == '>' || ft_isspace(cmd[*i])))
		(*i)++;
	while (cmd[*i] && !ft_isspace(cmd[*i]))
		(*i)++;
}

char	*before_red(char *cmd, int *i)
{
	int		start;
	int		j;
	char	*bef_red;

	start = *i;
	j = 0;
	while (cmd[*i] && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	if (start == *i)
		return (NULL);
	bef_red = malloc((*i - start + 1));
	if (!bef_red)
	{
		perror("Failed to allocate memory for bef_red");
		exit(EXIT_FAILURE);
	}
	while (start < *i)
	{
		bef_red[j] = cmd[start];
		start++;
		j++;
	}
	bef_red[j] = '\0';
	return (bef_red);
}

char	*remove_redirects(char *cmd)
{
	char	*clean_cmd;
	char	*temp;
	int		i;

	i = 0;
	clean_cmd = NULL;
	while (cmd[i])
	{
		temp = before_red(cmd, &i);
		join_part(&clean_cmd, temp);
		redir_part(cmd, &i);
	}
	return (clean_cmd);
}
