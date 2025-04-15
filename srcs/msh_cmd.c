/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:26:24 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/15 18:42:05 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin_str(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "cd", 2) == 0 && (cmd[2] == ' ' || cmd[2] == '\0'))
		return (1);
	if (ft_strncmp(cmd, "echo", 4) == 0 && (cmd[4] == ' ' || cmd[4] == '\0'))
		return (1);
	if (ft_strncmp(cmd, "env", 3) == 0 && (cmd[3] == ' ' || cmd[3] == '\0'))
		return (1);
	if (ft_strncmp(cmd, "exit", 4) == 0 && (cmd[4] == ' ' || cmd[4] == '\0'))
		return (1);
	if (ft_strncmp(cmd, "export", 6) == 0 && (cmd[6] == ' ' || cmd[6] == '\0'))
		return (1);
	if (ft_strncmp(cmd, "unset", 5) == 0 && (cmd[5] == ' ' || cmd[5] == '\0'))
		return (1);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && (cmd[3] == ' ' || cmd[3] == '\0'))
		return (1);
	return (0);
}

int	is_builtin(char *cmd)
{
	char	*clean_cmd;
	int		result;

	clean_cmd = remove_redirects(cmd);
	if (!clean_cmd)
		return (0);
	clean_cmd = remove_quotes_first_word(clean_cmd);
	if (!clean_cmd)
		return (0);
	result = is_builtin_str(clean_cmd);
	free(clean_cmd);
	return (result);
}

int	run_builtin(char *cmd, int fd)
{
	if (ft_strncmp(cmd, "exit", 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' '))
		return (handle_exit(cmd, NULL), 1);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && (cmd[3] == '\0' || cmd[3] == ' '))
		return (ft_getcwd(cmd, fd));
	if (ft_strncmp(cmd, "cd", 2) == 0 && (cmd[2] == '\0' || cmd[2] == ' '))
		return (handle_cd(cmd));
	if (ft_strncmp(cmd, "echo", 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' '))
		return (handle_echo(cmd, fd));
	if (ft_strncmp(cmd, "env", 3) == 0 && (cmd[3] == '\0' || cmd[3] == ' '))
		return (print_env(fd));
	if (ft_strncmp(cmd, "export", 6) == 0 && (cmd[6] == '\0' || cmd[6] == ' '))
		return (handle_export(cmd, fd));
	if (ft_strncmp(cmd, "unset", 5) == 0 && (cmd[5] == '\0' || cmd[5] == ' '))
		return (handle_unset(cmd, fd));
	return (1);
}

void	execute_builtin(char *cmd, int fd, int *status)
{
	struct s_saved_std	std;
	char				*clean_cmd;

	*status = 1;
	if (!cmd)
		return ;
	clean_cmd = remove_quotes_first_word(remove_redirects(cmd));
	if (!clean_cmd)
		return ;
	std.saved_stdin = dup(STDIN_FILENO);
	std.saved_stdout = dup(STDOUT_FILENO);
	handle_redirection(cmd, status);
	*status = run_builtin(clean_cmd, fd);
	free(clean_cmd);
	dup2(std.saved_stdin, STDIN_FILENO);
	dup2(std.saved_stdout, STDOUT_FILENO);
	close(std.saved_stdin);
	close(std.saved_stdout);
}
