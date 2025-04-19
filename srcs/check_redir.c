/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:31:09 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/18 20:16:37 by alex             ###   ########.fr       */
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

void	handle_heredoc_child(int write_fd, const char *delimiter, int *status, char **envp)
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
		check_quastion_sign(&line, *status);
		bridge_var(&line, envp);
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	exit(0);
}
