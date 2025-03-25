/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:14:00 by alex              #+#    #+#             */
/*   Updated: 2025/03/25 18:45:17 by tfarkas          ###   ########.fr       */
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

#include "lib/get_next_line/get_next_line.h"
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
	int		pipecount;
	int		pidcount;
	int		cmd_num;
	pid_t	*childs;
	int		save_stdin;
	int		save_stdout;
	int		fd_readl;
	int		fd_exin;
	int		fd_exout;
	int		pipefd[2];
}	t_store;

typedef struct s_line
{
	char		**redir_l;
	char		*cmd_l;
	char		**redir_parts;
	t_tokentype	*tokarr;
	int			pipecount;
	int			cmd_num;
	int			fd_redin;
	int			fd_redout;
}	t_line;


//minishell.c
void	remove_chars(char **str, char ch);
void	execute_pipe_commands(char *cmd, int fd, int *status);
int		is_nummeric(char *line);
void	handle_exit(char *line, int *status);
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
void	run_ex(char **line, int *status);
void	dis_echo_insc(struct termios *old_term);
void	restore_echo(struct termios *old_term);
int 	main(void);

//msh_redirect.c
int		red_in(char *fd_name);
int		red_out(char *fd_name);
int		red_out_append(char *fd_name);
void	read_in_temp(char *delimeter, int fd_delimeter);
int		red_del(char *delimeter);

//msh_redir_cmd_call.c
int		count_delimeter(char *line, char delimeter);
void	redir_prep(char *filename, char delimeter, int count, t_store *st);
void	redir_case(char *line, int *i, t_store *st);
void	cmd_case(char *line, char *cmd, int *i, int *cmdlen);
int		redir_cmd_s(char *line, t_store *st);

//msh_operations.c
// void	choose_redirection(t_tokentype e_red, char *name_d, t_store *st);
void	ch_red(t_tokentype e_red, char *name_d, t_line *sline);
void	run_red_choose(t_line *sline);
int		count_rps(char *redir);
void	set_red(char *redir, t_tokentype e_red, int *i);
char	*save_name_d(char *redir, t_tokentype e_red);
void	redir_ch(t_line *sline, char *redir);
void	redir_line(t_line *sline);

// void	set_red(char *redir, t_tokentype e_red, int *i);
// void	redir_ch(char *redir, t_tokentype e_red);
// void	redir_line(t_line *sline);


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
int		execute_cmd(char *cmd, t_store *st);
int		is_builtin(char *cmd);
void	execute_builtin(char *cmd, int fd, int *status);
int 	builtin_check(char *cmd, t_store *st, int *status);
int		choose_cmd(char *line, t_store *st);

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
// void	pipe_dup(int pipefd[2], int which, char *beforep, char *afterp);
// void	ft_pipe(char *beforep, char *afterp);
// void	ft_pipe(t_store *st);
void	pipe_read(t_store *st);
void	pipe_write(t_store *st);
void	chproc_fd(t_store *st);
void	gnl_readline(t_store *st, int *status);
void 	temp_readline(char *line, t_store *st);
int		read_readline(t_store *st);

//msh_redir_cmd_utils.c
void	init_store(t_store	*st);
void	reset_fds(t_store *st);
int		cmd_fds_reset(char **cmd, t_store *st);
void	save_chpid(pid_t pid, t_store *st);
int		wait_child(t_store *st, int status);

//msh_red_con.c
void	init_line(t_line *sline);
void	free_line(t_line *sline);
char	*ft_crjoin(char	*s1, char *s2);
void	store_redir(char *line, int *i, t_line *sline);
void	store_cmd(char *line, int *i, t_line *sline);
void	store_lines(char *line, t_line *sline);


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


/*
The following file includes functions for checking 
state of different aspect of minishell
*/
typedef enum e_fds
{
	SAVE_STDIN,
	SAVE_STDOUT,
	FD_READL,
	FD_EXIN,
	FD_EXOUT,
	PIPEFD_W,
	PIPEFD_R
}	t_fds;

typedef enum e_wopt
{
	START_FD,
	END_FD,
	OPEN_FD,
	CLOSE_FD,
	DUPLICATE_FD,
}	t_wopt;

//test_funcs.c
void	fds_state(void);
void	check_tty();
void	fds_state_f(FILE *fd_monit);
void	init_fds_struct(int fd_struct[6], t_store *st);
void	struct_fds_state(FILE *fd_monit, t_store *st);
void 	check_tty_f(FILE *fd_monit);
void	delete_file(char *monitor_file);
void	wr_openfd(int fd, char *name, FILE *fd_monit, t_wopt wopt);
void	fds_type(t_fds fds, t_store *st, int *fd, char **name);
void	monitor_fds(t_store *st, t_fds fds, t_wopt wopt);

#endif