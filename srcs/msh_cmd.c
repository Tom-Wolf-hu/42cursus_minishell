/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:26:24 by tfarkas           #+#    #+#             */
/*   Updated: 2025/02/21 21:06:35 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*shearch_cmd(char *cmd)
{
	char	*result;
	char	*env_varval;
	char	**env_path;
	int		i;

	i = 0;
	env_varval = env_variable_val("PATH");
	if (!env_varval)
		return (free(env_varval), NULL);
	env_path = dev_strarr(env_varval, ':');
	if (!env_path)
		return (NULL);
	while (env_path[i] != NULL)
	{
		result = cmd_acces(env_path[i], cmd);
		if (result)
			break ;
		i++;
	}
	free_arr(env_path);
	return (free(env_varval), result);
}

char	*cmd_path(char *cmd)
{
	char	*path;
	char	**arg;

	path = NULL;
	if (cmd[0] == '/' || ft_strncmp("./", cmd, 2) == 0)
		path = ft_strdup(cmd);
	else
	{
		arg = dev_strarr(cmd, ' ');
		if (arg && arg[0])
			path = shearch_cmd(arg[0]);
	}
	if (!path || access(path, X_OK) < 0)
		return (NULL);
	return (path);
}

int	execute_cmd(char *cmd)
{
	pid_t		pid;
	int			status;
	char		*cmdp;
	char		**cmdarg;
	extern char	**environ;
	// char	*args[] = {cmd, NULL};

	cmdp = cmd_path(cmd);
	cmdarg = dev_strarr(cmd, ' ');
	pid = fork();
	if (pid == 0 && cmdp && cmdarg)
	{
		execve(cmdp, cmdarg, environ);
		perror("Failed to execute the command.\n");
		exit(127);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("Failed to create fork.\n");
	return(WEXITSTATUS(status));
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
void	choose_cmd(char *line)
{
	static int	status = 0;
	// char		*new_line;

	// new_line = remove_first_spaces(line, status);
	if (is_empty(line))
		return ;
	if (is_builtin(line) == 0)
		return ;
	else
		status = execute_cmd(line);
	// else
		// printf("minishell: command not found: %s\n", line);
}

