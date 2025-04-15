/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:31:09 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/15 20:01:04 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	gf_name_length(char *cmd, int *i, int *start)
{
	while (cmd[*i] && (cmd[*i] == '>' || cmd[*i] == '<'))
		(*i)++;
	while (cmd[*i] && (cmd[*i] == ' ' || cmd[*i] == '\t' || cmd[*i] == '\n'))
		(*i)++;
	if (!cmd[*i])
		return (1);
	*start = *i;
	while (cmd[*i] && cmd[*i] != ' ' && cmd[*i] != '\t'
		&& cmd[*i] != '\n' && cmd[*i] != '<' && cmd[*i] != '>')
		(*i)++;
	return (0);
}

char	*get_filename(char *cmd)
{
	int		i;
	int		j;
	int		start;
	char	*clean_filename;
	char	*filename;

	i = 0;
	j = 0;
	filename = NULL;
	if (gf_name_length(cmd, &i, &start))
		return (NULL);
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

void	handle_heredoc_child(int write_fd, const char *delimiter)
{
	char	*line;

	reset_stdin();
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || (ft_strlen(line) == ft_strlen((char *)delimiter)
				&& ft_strncmp(line, delimiter,
					ft_strlen((char *)delimiter)) == 0))
		{
			free(line);
			break ;
		}
		check_quastion_sign(&line, ft_itoa(g_status));
		bridge_var(&line);
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	exit(0);
}


// char	*get_filename(char *cmd)
// {
// 	int		i = 0, j = 0, start;
// 	char	*clean_filename;
// 	char	*filename;

// 	filename = NULL;
// 	while (cmd[i] && (cmd[i] == '>' || cmd[i] == '<'))
// 		i++;
// 	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n'))
// 		i++;
// 	if (!cmd[i])
// 		return (NULL);
// 	start = i;
// 	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n' && cmd[i] != '<' && cmd[i] != '>')
// 		i++;
// 	filename = malloc(i - start + 1);
// 	if (!filename)
// 	{
// 		perror("malloc");
// 		exit(EXIT_FAILURE);
// 	}
// 	while (start < i)
// 		filename[j++] = cmd[start++];
// 	filename[j] = '\0';
// 	clean_filename = remove_quotes_first_word(filename);
// 	// printf("clean_filename: %s\n", clean_filename);
// 	return (clean_filename);
// 	// return filename;
// }

// void	handle_heredoc(const char *delimiter) // ВЫОДИТЬ НИЧЕГО НЕ НАДО!!!
// {
// 	int pipe_fd[2];
// 	char *line = NULL;

// 	if (pipe(pipe_fd) == -1) // Создаём пайп
// 	{
// 		perror("pipe");
// 		return (exit(1));
// 	}
// 	// fprintf(stderr, "start heredoc\n");
// 	// printf("pipefd[0] == %d; pipefd[1] == %d\n", pipe_fd[0], pipe_fd[1]);
// 	// printf("delimiter: %s; len: %d\n", delimiter, ft_strlen(delimiter));
// 	g_heredoc = 1;
// 	while (g_heredoc)
// 	{
// 		// write(STDOUT_FILENO, "> ", 2);
// 		// printf("g_heredoc: %d\n", g_heredoc);
// 		line = readline("heredoc> "); // НУЖНО ЗАПОМИНАТЬ LINE, ЧТОБЫ ЕГО ПОТОМ ВЫВЕСТИ
// 		// if (g_heredoc == 0)
// 		// {
// 		// 	free(line);
// 		// 	line = delimiter;
// 		// }
// 		if (!line || (ft_strncmp(line, delimiter, ft_strlen((char *)delimiter)) == 0 && ft_strlen(line) == ft_strlen((char *)delimiter)))
// 		{
// 			free(line);
// 			break ;
// 		}
// 		if (g_heredoc == 0)
// 		{
// 			// write(2, "passed1\n", 8);
// 			// write(pipe_fd[1], delimiter, ft_strlen((char *)delimiter));
// 			// write(pipe_fd[1], "\n", 1);
// 			free(line);
// 			break ;
// 		}
// 		// printf("line: %s; len: %d\n", line, ft_strlen(line));
// 			// добавить еще условие на && line[strlen(delimiter)] == '\n'
// 		write(pipe_fd[1], line, strlen(line)); // Пишем в пайп
// 		write(pipe_fd[1], "\n", 1);
// 		free(line);
// 	}
// 	g_heredoc = 0;
// 	close(pipe_fd[1]);
// 	dup2(pipe_fd[0], STDIN_FILENO); // Перенаправляем stdin на пайп
// 	close(pipe_fd[0]);
// }

/*
	Old version of handle_redirection
*/

// void	handle_redirection(char *cmd, int *status)
// {
// 	int		file_fd;
// 	char	*filename;

// 	file_fd = -1;
// 	filename = get_filename(cmd);
// 	if (!filename)
// 		return ;
// 	if (strstr(cmd, "<<") && !strstr(cmd, "<<<"))
// 	{
// 		// printf("redirection for <<\n");
// 		handle_heredoc(filename);
// 	}
// 	else if ((strstr(cmd, ">>")))
// 	{
// 		// printf("redirection for >>\n");
// 		file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		if (file_fd == -1)
// 			return (perror("open"), exit(1));
// 		dup2(file_fd, STDOUT_FILENO);
// 		close(file_fd);
// 	}
// 	else if ((strchr(cmd, '>')))
// 	{
// 		// printf("redirection for >\n");
// 		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (file_fd == -1)
// 			return (perror("open"), exit(1));
// 		dup2(file_fd, STDOUT_FILENO);
// 		close(file_fd);
// 	}
// 	else if ((strchr(cmd, '<')))
// 	{
// 		file_fd = open(filename, O_RDONLY);
// 		if (file_fd == -1)
// 		{
// 			perror("open");
// 			*status = 1;
// 			return ;
// 		}
// 		dup2(file_fd, STDIN_FILENO);
// 		close(file_fd);
// 	}
// 	free(filename);
// }

// char	*remove_redirects(char *cmd)
// {
// 	char	*clean_cmd;
// 	char	*temp;
// 	int		start;
// 	int		end;
// 	int		i;
// 	int		j;

// 	j = 0;
// 	start = 0;
// 	while (cmd[start] && cmd[start] != '>' && cmd[start] != '<')
// 		start++;     // Находим первый символ редиректа
// 	if (!cmd[start]) // Если редиректов нет, просто копируем строку
// 		return (strdup(cmd));
// 	end = start;
// 	while (cmd[end] && (cmd[end] == '>' || cmd[end] == '<'
// 			|| isspace(cmd[end])))
// 		end++;                             // Пропускаем пробелы и сам редирект
// 	while (cmd[end] && !isspace(cmd[end])) // Пропускаем имя файла
// 		end++;
// 	i = 0;
// 	while (cmd[i] && i < start) // Копируем всё до редиректа
// 		i++;
// 	while (cmd[end] && isspace(cmd[end])) // Пропускаем лишние пробелы после имени файла
// 		end++;
// 	clean_cmd = malloc(strlen(cmd) - (end - start) + 1);
// 	if (!clean_cmd)
// 		return (NULL);
// 	j = 0;
// 	i = 0;
// 	while (cmd[i] && i < start) // Копируем всё до редиректа
// 		clean_cmd[j++] = cmd[i++];
// 	while (cmd[end] && cmd[end] != '>' && cmd[end] != '<')
// 		clean_cmd[j++] = cmd[end++];
// 	clean_cmd[j] = '\0';
// 	// printf("clean_cmd in remove redirects: %s\n", clean_cmd);
// 	return (clean_cmd);
// }