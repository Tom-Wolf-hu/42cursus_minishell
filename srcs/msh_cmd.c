/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:26:24 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/16 14:45:52 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin_choose(char *clean_cmd)
{
	if (ft_strcmp(clean_cmd, "exit") == 0
		|| ft_strncmp(clean_cmd, "exit ", 5) == 0)
		return (1);
	if (ft_strncmp(clean_cmd, "pwd", 3) == 0
		|| ft_strncmp(clean_cmd, "pwd ", 4) == 0)
		return (1);
	else if (ft_strncmp(clean_cmd, "cd ", 3) == 0
		|| ft_strcmp(clean_cmd, "cd") == 0)
		return (1);
	else if (ft_strncmp(clean_cmd, "echo ", 5) == 0
		|| ft_strcmp(clean_cmd, "echo") == 0)
		return (1);
	else if (ft_strncmp(clean_cmd, "env", 3) == 0
		|| ft_strncmp(clean_cmd, "env ", 4) == 0)
		return (1);
	else if (ft_strncmp(clean_cmd, "export ", 7) == 0
		|| ft_strcmp(clean_cmd, "export") == 0)
		return (1);
	else if (ft_strncmp(clean_cmd, "unset ", 6) == 0
		|| ft_strcmp(clean_cmd, "unset") == 0)
		return (1);
	return (0);
}

int	is_builtin(char *cmd)
{
	char	*clean_cmd;

	clean_cmd = remove_redirects(cmd);
	if (!clean_cmd)
		return (0);
	clean_cmd = remove_quotes_first_word(clean_cmd);
	if (!clean_cmd)
		return (0);
	if (is_builtin_choose(clean_cmd) == 1)
		return (free(clean_cmd), 1);
	return (free(clean_cmd), 0);
}

void	exec_builtin_choose(char *clean_cmd, int fd, int *status, struct s_saved_std *std)
{
	if (ft_strcmp(clean_cmd, "exit") == 0
		|| ft_strncmp(clean_cmd, "exit ", 5) == 0)
		handle_exit(clean_cmd, status, std);
	if (ft_strcmp(clean_cmd, "pwd") == 0
		|| ft_strncmp(clean_cmd, "pwd ", 4) == 0)
		*status = ft_getcwd(clean_cmd, fd);
	else if (ft_strncmp(clean_cmd, "cd ", 3) == 0
		|| ft_strcmp(clean_cmd, "cd") == 0)
		*status = handle_cd(clean_cmd);
	else if (ft_strncmp(clean_cmd, "echo ", 5) == 0
		|| ft_strcmp(clean_cmd, "echo") == 0)
		*status = handle_echo(clean_cmd, fd);
	else if (ft_strcmp(clean_cmd, "env") == 0
		|| ft_strcmp(clean_cmd, "env ") == 0)
		*status = print_env(fd);
	else if (ft_strncmp(clean_cmd, "export ", 7) == 0
		|| ft_strcmp(clean_cmd, "export") == 0)
		*status = handle_export(clean_cmd, fd);
	else if (ft_strncmp(clean_cmd, "unset ", 6) == 0
		|| ft_strcmp(clean_cmd, "unset") == 0)
		*status = handle_unset(clean_cmd, fd);
}

void	execute_builtin(char *cmd, int fd, int *status)
{
	struct s_saved_std	std;
	char				*clean_cmd;

	*status = 1;
	if (!cmd)
		return ;
	clean_cmd = remove_redirects(cmd);
	if (!clean_cmd)
		return ;
	clean_cmd = remove_quotes_first_word(clean_cmd);
	if (!clean_cmd)
		return ;
	std.saved_stdin = dup(STDIN_FILENO);
	std.saved_stdout = dup(STDOUT_FILENO);
	handle_redirection(cmd, status);
	exec_builtin_choose(clean_cmd, fd, status, &std);
	free(clean_cmd);
	dup2(std.saved_stdin, STDIN_FILENO);
	dup2(std.saved_stdout, STDOUT_FILENO);
	close(std.saved_stdin);
	close(std.saved_stdout);
}
