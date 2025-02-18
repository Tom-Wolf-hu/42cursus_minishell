/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:14:00 by alex              #+#    #+#             */
/*   Updated: 2025/02/18 18:26:41 by tfarkas          ###   ########.fr       */
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
	t_tokentype	toktype;
	char		*strpart;
	t_pnode		*next;
	t_pnode		*prev;
}	t_pnode;

//minishell.c
void	ft_error(void);
void	sig_handler(int sig);
int 	main(void);

//msh_redirect.c
void	red_in(char *fd_name);
void	red_out(char *fd_name);
void	red_out_append(char *fd_name);
void	read_in_temp(char *delimeter, int fd_delimeter);
void	red_del(char *delimeter);

//msh_operations.c
void	choose_op(t_tokentype e_red, char *name_d);

//msh_parse.c
void	add_node(t_pnode **node);
void	nl_clear(t_pnode **node);
void	back_to_firstnode(t_pnode **node);

#endif