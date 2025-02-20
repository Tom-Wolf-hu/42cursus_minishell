/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:26:24 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/20 14:54:07 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*cmd_env_path(char *cmd, char **env)
{
	
}

char	*cmd_path(char *cmd, char **env)
{
	char	*path;

	if (cmd[0] == '/' || ft_strncmp("./", cmd, 2) == 0)
		path = ft_strdup(cmd);
	else
		path = cmd_env_path(cmd, env);
	if (!path || access(path, X_OK) < 0)
		return (NULL);
	return (path);
}

void	execute_cmd(char *cmd, char **env)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	*args[] = {cmd, NULL};

	pid = fork();
	if (pid == 0)
	{
		execve("/usr/bin/ls", args, env);
		perror("execve");
		exit(127);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork");
}

int	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "pwd") == 0)
		ft_getcwd(cmd);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		handle_cd(cmd);
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		handle_echo(cmd);
	else if (ft_strcmp(cmd, "env") == 0)
		print_env();
	else if (ft_strncmp(cmd, "export", 6) == 0)
		handle_export(cmd);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		handle_unset(cmd);
	else
		return (-1);
	return (0);
}

/*
My recommendation is that check for empty line we should later put in a previous checking funtion.
I kept this part, still we didn't reach that phase.
I'm little bit confused how the is_empty funtcion works. I would like to discuss it.
*/
void	choose_cmd(char *cmd, char **env)
{
	if (is_empty(cmd))
		return ;
	if (is_builtin(cmd) == 0)
		return ;
	else
		execute_cmd(cmd, **env);
	// else
		// printf("minishell: command not found: %s\n", line);
}

