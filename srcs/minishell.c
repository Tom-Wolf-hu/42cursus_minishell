/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/02/26 14:28:00 by omalovic         ###   ########.fr       */
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
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

int	ft_getcwd(char *line)
{
	char	buffer[128];
	size_t	size;

	size = 128;
	if (getcwd(buffer, size) == NULL)
	{
		perror("minishell: getcwd");
		return (free(line), exit(1), 1);
	}
	printf("%s\n", buffer);
	return (0);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

int	handle_cd(char *line)
{
	char	**arr;
	char	*path;
	int 	i;

	i = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1), 1);
	while (arr[i])
		i++;
	if (i > 2)
	{
		printf("minishell: cd: too many arguments\n");
		free_arr(arr);
		return (1);
	}
	if (i == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			free_arr(arr);
			return (1);
		}
	}
	else
		path = arr[1];
	if (chdir(path) != 0)
		return (perror("minishell: cd"), 1);
	free_arr(arr);
	return (0);
}

int	print_env(void)
{
	int				i;
	extern	char	**environ;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

int	check_line(char *line, int i)
{
	int	len;

	len = ft_strlen(line);
	while (i < len)
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*remove_first_spaces(char *line)
{
	char	*new_line;
	int		i;

	i = 0;
	new_line = NULL;
	if (is_empty(line))
		return (new_line);
	while (line[i] && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'
		|| line[i] == '\v' || line[i] == '\f' || line[i] == '\r'))
	{
		i++;
	}
	new_line = ft_strdup(line + i);
	return (new_line);
}

void	disable_ctrl_c_output(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	setup_signal_handlers(void)
{
	struct sigaction sa;

	sa.sa_handler = sig_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

int main(void)
{
	char			*line;
	static int		status;

	disable_ctrl_c_output();
	setup_signal_handlers();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, "exit") == 0)
			break ;
		else if (ft_strcmp(line, "clear") == 0)
			rl_clear_history();
		else
		{
			add_history(line);
			check_quastion_sign(&line, ft_itoa(status));
			bridge_var(&line);
			status = choose_cmd(line);
		}
		free(line);
	}
	rl_clear_history();
	free(line);
}

/* 
найти переменную в строке
выяснить длину значения
выделить память (минус длина названия переменной, плюс длина значения)
переписать массив и начиная с доллара начинать вписывать значение */