/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/04/29 13:38:08 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile int	g_status = 0;

void	sig_handler(int sig)
{
	int		status;
	pid_t	pid;

	if (sig == SIGINT)
	{
		g_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		pid = waitpid(-1, &status, WNOHANG);
		if (pid == 0)
			return ;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_empty_command(char *cmd, int *status, char **envp)
{
	struct s_saved_std	std;
	char				*line;

	std.saved_stdout = dup(STDOUT_FILENO);
	std.saved_stdin = dup(STDIN_FILENO);
	handle_redirection(cmd, status, envp);
	if (*status == 1)
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
		clean_cmd2 = remove_quotes_first_word(cmd_arr[i]);
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

void	run_ex(char **line, int *status, char ***myenvp)
{
	char	**arr;
	int		i;
	char	*p;

	if (is_empty(*line))
		return ;
	add_history(*line);
	if (check_quotes(*line) == 1)
		return ;
	i = 0;
	while ((*line)[i] && ft_isspace((*line)[i]))
		i++;
	if ((*line)[i] == '\0')
		return ;
	p = ft_strdup(*line + i);
	if (check_quastion_sign(&p, *status))
		return (free(p));
	bridge_var(&p, *myenvp);
	if (ft_strcmp(p, "clear") == 0
		|| ft_strncmp(p, "clear ", 6) == 0)
		return (rl_clear_history(), free(p));
	if (!ft_strchr(p, '|'))
		return (execute_command_single(p, status, myenvp), free(p));
	return (execute_pipe_commands(p, status, myenvp), free(p));
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	static int		status = 0;
	char			**myenvp;

	myenvp = copy_arr(envp);
	disable_ctrl_c_output(&status);
	setup_signal_handlers();
	while (1)
	{
		if (isatty(STDIN_FILENO))
			line = readline("> ");
		else
			handle_gnl(&line, &status);
		change_status(&status);
		if (!line)
			handle_exit(ft_strdup("exit"), &status, NULL, myenvp);
		if (ft_strcmp(line, "exit") == 0
			|| ft_strncmp(line, "exit ", 5) == 0)
			handle_exit(line, &status, NULL, myenvp);
		else
			run_ex(&line, &status, &myenvp);
		free(line);
	}
	rl_clear_history();
}

// ps aux | grep bash | awk '{print $2}'    снова не работает		+
// Введите Ctrl+C внутри minishell (появляется лишний > в приглашении) +
// sleep 5

/* 
# Введите Ctrl+C внутри minishell (появляется лишний > в приглашении) +
sleep 5	(через раз)

ctrl + \ after some stuff should do nothing
если писать cat и затем нажимать ctrl+c появляется лишний > (иногда) +


> export PATH=/usr/bin					+
zsh: segmentation fault  ./minishell


> cat <filesfa    (виснит)			+
clean_filename: filesfa
filename: filesfa
open: No such file or directory

> 

> cat <<		(должна быть просто ошибка)	+

> | ls			(должна быть просто ошибка) +
> ls |			(должна быть просто ошибка) +
> ls || grep test							+

НЕ ДОЛЖНА ИНТЕРПРЕТИРОВАТЬ ЭТИ СИМВОЛЫ
echo hello; ls			+
echo hello \			+
echo \			+
echo -nnn hello			+

$nonexist | wc -l	+-
$nonexist			+-

> ./minishell
> cat << EOF
cat : Command not found

echo "hello"$ +

echo $? $USER $PATH $$$


valgrind:
bridge_var

*/