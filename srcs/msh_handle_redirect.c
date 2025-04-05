/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_handle_redirect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 12:41:13 by tfarkas           #+#    #+#             */
/*   Updated: 2025/03/31 13:49:28 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_redir_sign(char *cmd, int *i)
{
	int		start;
	int		j;
	char	*rs;

	j = 0;
	while (cmd[*i])
	{
		if (cmd[*i] == '>' || cmd[*i] == '<')
			break ;
		i++;
	}
	start = *i;
	while (cmd[*i] && (cmd[*i] == '>' || cmd[*i] == '<'))
		i++;
	if (*i <= start)
		return (NULL);
	if (*i > start && *i - start <= 2)
	{
		rs = (char *)malloc((*i - start + 1) * sizeof(char));
		if (!rs)
		{
			perror("Failed to allocate memory for rs");
			exit(EXIT_FAILURE);
		}
		while (j < *i - start)
		{
			rs = cmd[start + j];
			j++;
		}
		rs[j] = '\0';
	}
	else
	{
		perror("syntax error: to many redirection sign");
		exit(EXIT_FAILURE);
	}
	return (rs);
}

char	*get_filename(char *cmd, char **rs)
{
	char	*filename;
	int		j;
	int		i;

	i = 0;
	filename = NULL;
	*rs = get_redir_sign(cmd, &i);
	if (*rs == NULL)
		return (NULL);
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
	fprintf(stderr, "filename in get_filaname function: %s\n", filename);
	return (filename);
}

void	handle_redirection(char *cmd, int *status)
{
	int		file_fd;
	char	*filename;
	char	*rs;

	file_fd = -1;
	rs = NULL;
	filename = get_filename(cmd, &rs);
	if (!filename)
		return ;
	if (strstr(cmd, "<<") && !strstr(cmd, "<<<"))
	{
		fprintf(stderr, "redirection for <<%s\n", filename);
		handle_heredoc(filename);
	}
	else if ((strstr(cmd, ">>")))
	{
		fprintf(stderr, "redirection for >>%s\n", filename);
		file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (file_fd == -1)
			return (perror("open"), exit(1));
		dup2(file_fd, STDOUT_FILENO);
		close(file_fd);
	}
	else if ((strchr(cmd, '>')))
	{
		fprintf(stderr, "redirection for >%s\n", filename);
		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (file_fd == -1)
			return (perror("open"), exit(1));
		dup2(file_fd, STDOUT_FILENO);
		close(file_fd);
	}
	else if ((strchr(cmd, '<')))
	{
		fprintf(stderr, "redirection for <%s\n", filename);
		file_fd = open(filename, O_RDONLY);
		if (file_fd == -1)
		{
			perror("open");
			// *status = 1;
			return ;
		}
		dup2(file_fd, STDIN_FILENO);
		close(file_fd);
	}
}