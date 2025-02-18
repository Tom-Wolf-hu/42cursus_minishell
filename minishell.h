/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:14:00 by alex              #+#    #+#             */
/*   Updated: 2025/02/18 12:46:05 by tfarkas          ###   ########.fr       */
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

typedef enum e_redir
{
	REDINPUT,
	REDOUTPUT,
	APPENDREDOUTPUT,
	REDDELIMETER
}	t_redir;

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

#endif