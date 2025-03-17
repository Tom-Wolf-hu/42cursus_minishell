/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/03/17 16:17:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error(char *error, int exit_status)
{
	perror(error);
	exit(exit_status);
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

int	check_line(char *line, int i)
{
	int	len;

	if (!line)
		return (1);
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

void	disable_ctrl_c_output(int *status)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	*status = 130;
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

int	is_nummeric(char *line)
{
	int i = 0;

	if (!line)
		return (1);
	while (line[i])
	{
		if (line[i] < '0' || line[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	run_ex(char **line, int *status)
{
	t_store	st;

	init_store(&st);
	add_history(*line);
	check_quastion_sign(line, ft_itoa(*status));
	bridge_var(line);
	execute_builtin(*line, 0, status);
	// temp_readline(*line, &st);
	// *status = read_readline(&st);
}

int main(void)
{
	char			*line;
	static int		status = 0;

	// check_tty();
	disable_ctrl_c_output(&status);
	setup_signal_handlers();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, "exit") == 0 || ft_strncmp(line, "exit ", 5) == 0)
		{
			handle_exit(line, &status);
			break ;
		}
		else if (ft_strcmp(line, "clear") == 0 || ft_strncmp(line, "clear ", 6) == 0)
			rl_clear_history();
		else
			run_ex(&line, &status);
		free(line);
	}
	rl_clear_history();
	free(line);
}
// status after ctrl + c