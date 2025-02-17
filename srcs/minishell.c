/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/02/17 16:54:48 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error(void)
{
	perror("Error");
	exit(1);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	ft_getcwd(char *line)
{
	char	buffer[128];
	size_t	size;

	size = 128;
	if (getcwd(buffer, size) == NULL)
	{
		perror("minishell: getcwd");
		free(line);
		exit(1);
	}
	printf("%s\n", buffer);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	handle_cd(char *line)
{
	char	**arr;
	char	*path;
	int 	i;

	i = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1));
	while (arr[i])
		i++;
	if (i > 2)
	{
		printf("minishell: cd: too many arguments\n");
		free_arr(arr);
		return ;
	}
	if (i == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			free_arr(arr);
			return ;
		}
	}
	else
		path = arr[1];
	if (chdir(path) != 0)
		perror("minishell: cd");
	free_arr(arr);
}

void	mywrite(char *line)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	if (line[i] == 34)
		i++;
	if (line[len - 1] == 34)
		len--;
	while (i < len)
	{
		write(1, &line[i], 1);
		i++;
	}
}

void	show_input(char **arr, int flag)
{
	int	i;
	int len;

	if (flag)
		i = 2;
	else
		i = 1;
	len = 0;
	while (arr[len])
		len++;
	while (arr[i])
	{
		if (i == len - 1)
		{
			mywrite(arr[i]);
			if (!flag)
				write(1, "\n", 1);
			return ;
		}
		mywrite(arr[i]);
		write(1, " ", 1);
		i++;
	}
}

void	handle_echo(char *line)
{
	char	**arr;
	int		i;

	i = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1));
	while (arr[i])
		i++;
	if (i == 1)
		return (write(1, "\n", 1), free_arr(arr));
	if (i == 2)
	{
		if (ft_strcmp(arr[1], "-n") == 0)
			return (free_arr(arr));
		else
			return (mywrite(arr[1]), write(1, "\n", 1), (void)0);
	}
	if (i > 2)
	{
		if (ft_strcmp(arr[1], "-n") == 0)
			return (show_input(arr, 1), free_arr(arr));
		show_input(arr, 0);
	}
	free_arr(arr);
}

int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int	is_empty(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

extern	char	**environ;

void	print_env(void)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}

void	handle_export(char *line)
{
	
}

void	choose_cmd(char *line)
{
	if (is_empty(line))
		return ;
	if (ft_strcmp(line, "pwd") == 0)
		ft_getcwd(line);
	else if (ft_strncmp(line, "cd", 2) == 0)
		handle_cd(line);
	else if (ft_strncmp(line, "echo", 4) == 0)
		handle_echo(line);
	else if (ft_strcmp(line, "env") == 0)
		print_env();
	else
		printf("minishell: command not found: %s\n", line);
}

int main(void)
{
	char	*line;

	while (1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, sig_handler);
		line = readline("> ");
		if (!line || ft_strcmp(line, "exit") == 0)
			break ;
		else if (ft_strcmp(line, "clear") == 0)
			rl_clear_history();
		else
		{
			add_history(line);
			choose_cmd(line);
		}
		free(line);
	}
	rl_clear_history();
	free(line);
}
