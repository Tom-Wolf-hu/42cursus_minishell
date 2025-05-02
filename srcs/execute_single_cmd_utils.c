/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single_cmd_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 14:34:09 by tfarkas           #+#    #+#             */
/*   Updated: 2025/05/02 14:35:52 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_empty_command(char *cmd, int *status, char **envp)
{
	struct s_saved_std	std;
	char				*line;

	std.saved_stdout = dup(STDOUT_FILENO);
	std.saved_stdin = dup(STDIN_FILENO);
	if (handle_redirection(cmd, status, envp))
	{
		dup2(std.saved_stdin, STDIN_FILENO);
		dup2(std.saved_stdout, STDOUT_FILENO);
		close_saved_std(&std);
		return ;
	}
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		write(STDOUT_FILENO, line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	dup2(std.saved_stdin, STDIN_FILENO);
	dup2(std.saved_stdout, STDOUT_FILENO);
	close_saved_std(&std);
}

int	clean_command_args(char **cmd_arr)
{
	int		i;
	char	*clean_cmd2;

	i = 0;
	while (cmd_arr[i])
	{
		clean_cmd2 = remove_quotes(cmd_arr[i]);
		if (!clean_cmd2)
		{
			printf("%s: The command not found\n", cmd_arr[i]);
			free_arr(cmd_arr);
			return (1);
		}
		free(cmd_arr[i]);
		cmd_arr[i] = clean_cmd2;
		i++;
	}
	return (0);
}

char	*my_strchr_quotes(char *s, int c)
{
	unsigned char	ch;
	char			quote;

	ch = c;
	quote = 0;
	while (*s != '\0')
	{
		if ((*s == '\'' || *s == '\"'))
		{
			if (!quote)
				quote = *s;
			else if (quote == *s)
				quote = 0;
		}
		else if (*s == ch && !quote)
			return ((char *)s);
		s++;
	}
	if (*s == ch && !quote)
		return ((char *)s);
	return (NULL);
}
