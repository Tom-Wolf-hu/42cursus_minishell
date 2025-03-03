/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:14:00 by alex              #+#    #+#             */
/*   Updated: 2025/03/03 18:58:44 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdbool.h>

// #include "lib/get_next_line/get_next_line.h"
#include "lib/libft/libft.h"

typedef enum e_token
{
	REDINPUT,
	REDOUTPUT,
	APPENDREDOUTPUT,
	REDDELIMETER,
	PIPET,
	SINGLEQUOTE,
	DOUBLEQUOTE,
	EXPAND,
	COMMAND,
	ARGUMENT,
	WORD
}	t_tokentype;

typedef struct s_pnode
{
	t_tokentype		toktype;
	char			*strpart;
	struct s_pnode	*next;
	struct s_pnode	*prev;
}	t_pnode;

typedef	struct s_store
{
	int		save_stdin;
	int		save_stdout;
	pid_t	*childs;
	int		fd;
}	t_store;


//minishell.c
void	ft_error(char *error, int exit_status);
void	sig_handler(int sig);
int		ft_getcwd(char *line, int fd);
void	free_arr(char **arr);
int		handle_cd(char *line);
int		check_line(char *line, int i);
int		check_quastion_sign(char **line, char *status);
char	*remove_first_spaces(char *line);
void	disable_ctrl_c_output(int *status);
void	setup_signal_handlers(void);
int		print_env(int fd);
int 	main(void);

//msh_redirect.c
void	red_in(char *fd_name);
void	red_out(char *fd_name);
void	red_out_append(char *fd_name);
void	read_in_temp(char *delimeter, int fd_delimeter);
void	red_del(char *delimeter);

//msh_redir_cmd_call.c
int		count_delimeter(char *line, char delimeter);
void	redir_prep(char *filename, char delimeter, int count);
void	redir_case(char *line, int *i);
void	cmd_case(char *line, char *cmd, int *i, int *cmdlen);
int		redir_cmd_s(char *line);

//msh_operations.c
void	choose_redirection(t_tokentype e_red, char *name_d);
// int		count_deilemeter(char *line, char delimeter);
// char	*allocate_word(char *line, int len);
// char	*save_w(char *line, char delimeter);
// void	check_redirect(char *line);

//msh_parse.c
void	add_node(t_pnode **node);
void	nl_clear(t_pnode **node);
void	back_to_firstnode(t_pnode **node);

//msh_cmd.c
char	*shearch_cmd(char *cmd);
char	*cmd_path(char *cmd);
int		execute_cmd(char *cmd);
int		is_builtin(char *cmd, int fd);
int		choose_cmd(char *line, t_store  *st);

//msh_env.c
char	*cmd_acces(char *path, char *cmd);
char	**dev_strarr(char *str, char delimeter);
char	*env_variable_val(char *variable_name);

//msh_utils.c
void	print_arr(char **strarr);
void	*ft_realloc(void *oldptr, size_t oldsize, size_t newsize);
char	*str_realloc(char *oldstr, size_t newsize);

//msh_line_analizer.c
char	save_word(char *line, char *word, int *j);
void	analyze_line(char *line, int *j);
void	loop_analyzel(char *line);

//msh_pipe.c
void	pipe_dup(int pipefd[2], int which, char *beforep, char *afterp);
void	ft_pipe(char *beforep, char *afterp);

//msh_redir_cmd_utils.c
void	init_store(t_store	*st);
void	reset_fds(t_store *st);
int		cmd_fds_reset(char **cmd, t_store *st);

//check_line.c
int		ft_isoperator(int c);
int		ft_isspace(int c);
int		skip_whites(char *line, int	*i);
int		is_empty(char *line);

//check_var.c
int		get_var_name_size(char *str);
void	get_var_name(char *dest, char *str);
int		cmp_names(char *name1, char *name2);
char	*get_name(char *str);
char	*find_var_value(char *var_name);
void	change_str(char **str, char *name, char *value);
void	remove_var_name(char **str, char *name);
void	bridge_var(char **str);

//echo.c
void	mywrite(char *line, int fd);
void	show_input(char **arr, int fd, int flag);
int		handle_echo(char *line, int fd);

//handle_export_unset.c
int		find_var_in_env(char *name);
int		mysetenv(char *name, char *value);
int		handle_export(char *line, int fd);
int		my_unsetenv(char *name);
int		handle_unset(char *line, int fd);

#endif