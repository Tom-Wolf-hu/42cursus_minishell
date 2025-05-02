/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/05/02 14:45:23 by tfarkas          ###   ########.fr       */
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
		pid = waitpid(-1, &status, WNOHANG);
		if (pid == 0)
			return ;
		rl_on_new_line();
		rl_redisplay();
	}
}

void	run_ex(char **line, int *status, char ***myenvp)
{
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
	p = remove_quotes_first_word(*line + i);
	if (!p)
		return ;
	if (check_quastion_sign(&p, *status))
		return (free(p));
	bridge_var(&p, *myenvp);
	if (!p)
		return ;
	if (ft_strcmp(p, "clear") == 0
		|| ft_strncmp(p, "clear ", 6) == 0)
		return (rl_clear_history(), free(p));
	if (!my_strchr_quotes(p, '|'))
		return (execute_command_single(p, status, myenvp), free(p));
	return (execute_pipe_commands(p, status, myenvp), free(p));
}

void	change_status(int *status)
{
	if (g_status == 130)
	{
		*status = g_status;
		g_status = 0;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*line;
	static int		status = 0;
	char			**myenvp;

	(void) argc;
	(void) argv;
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
		else if (if_exit(line))
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
$nonexist			----------

> ./minishell
> cat << EOF
cat : Command not found

echo "hello"$ +

echo $? $USER $PATH $$$


valgrind:
bridge_var

"echo " "cat lol.c | cat > lol.c"




*/