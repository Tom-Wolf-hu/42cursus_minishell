/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:31:09 by omalovic          #+#    #+#             */
/*   Updated: 2025/03/28 12:37:04 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *remove_redirects(char *cmd)
{
    char *clean_cmd;
    int start;
    int end;
    int i;
    int j = 0;

    start = 0;
    while (cmd[start] && cmd[start] != '>' && cmd[start] != '<')
        start++;  // Находим первый символ редиректа
    if (!cmd[start])  // Если редиректов нет, просто копируем строку
        return strdup(cmd);
    end = start;
    while (cmd[end] && (cmd[end] == '>' || cmd[end] == '<' || isspace(cmd[end])))
        end++;  // Пропускаем пробелы и сам редирект
    while (cmd[end] && !isspace(cmd[end]))  // Пропускаем имя файла
        end++;
    i = 0;
    while (cmd[i] && i < start)  // Копируем всё до редиректа
        i++;
    while (cmd[end] && isspace(cmd[end]))  // Пропускаем лишние пробелы после имени файла
        end++;
    clean_cmd = malloc(strlen(cmd) - (end - start) + 1);
    if (!clean_cmd)
        return NULL;
    j = 0;
    i = 0;
    while (cmd[i] && i < start)  // Копируем всё до редиректа
        clean_cmd[j++] = cmd[i++];

    while (cmd[end])  // Копируем остаток команды после редиректа
        clean_cmd[j++] = cmd[end++];
    clean_cmd[j] = '\0';
    return clean_cmd;
}

char *get_filename(char *cmd)
{
	char *filename;
	int j;
	int i;

	i = 0;
	filename = NULL;
	while (cmd[i])
	{
		if (cmd[i] == '>' || cmd[i] == '<')
			break ;
		i++;
	}
	while (cmd[i] && (cmd[i] == '>' || cmd[i] == '<'))
		i++;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n'))
		i++;
	if (!cmd[i])
		return (NULL);
	j = 0;
	filename = malloc(ft_strlen(cmd) - i + 1);
	if (!filename)
		return (NULL);
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
	{
		filename[j] = cmd[i];
		i++;
		j++;
	}
	filename[j] = '\0';
	return (filename);
}

void	handle_heredoc(const char *delimiter)	// ВЫОДИТЬ НИЧЕГО НЕ НАДО!!!
{
	int pipe_fd[2];
	char *line = NULL;

	if (pipe(pipe_fd) == -1) // Создаём пайп
	{
		perror("pipe");
		return ;
	}
	// printf("pipefd[0] == %d; pipefd[1] == %d\n", pipe_fd[0], pipe_fd[1]);
	// printf("delimiter: %s; len: %d\n", delimiter, ft_strlen(delimiter));
	while (1)
	{
		// write(STDOUT_FILENO, "> ", 2);
		line = readline("> ");	// НУЖНО ЗАПОМИНАТЬ LINE, ЧТОБЫ ЕГО ПОТОМ ВЫВЕСТИ
		if (!line || strncmp(line, delimiter, strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		// printf("line: %s; len: %d\n", line, ft_strlen(line)); // добавить еще условие на && line[strlen(delimiter)] == '\n'
		write(pipe_fd[1], line, strlen(line)); // Пишем в пайп
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO); // Перенаправляем stdin на пайп
	close(pipe_fd[0]);
}

void	handle_redirection(char *cmd, int *status)
{
	int file_fd;
    char *filename;

    file_fd = -1;
    filename = get_filename(cmd);
    if (!filename)
        return;
	if (strstr(cmd, "<<") && !strstr(cmd, "<<<"))
	{
		// printf("redirection for <<\n");
		handle_heredoc(filename);
	}
	else if ((strstr(cmd, ">>")))
	{
		// printf("redirection for >>\n");
		file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (file_fd == -1)
			return (perror("open"), exit(1));
		dup2(file_fd, STDOUT_FILENO);
		close(file_fd);
	}
	else if ((strchr(cmd, '>')))
	{
		// printf("redirection for >\n");
		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file_fd == -1)
			return (perror("open"), exit(1));
		dup2(file_fd, STDOUT_FILENO);
		close(file_fd);
	}
	else if ((strchr(cmd, '<'))) // незнаю как затестить ---------------------------------------------------
	{
		// printf("redirection for <\n");
		file_fd = open(filename, O_RDONLY);
		if (file_fd == -1)
		{
			perror("open");
			// *status = 1;
			return;
		}
		dup2(file_fd, STDIN_FILENO);
		close(file_fd);
	}
}
