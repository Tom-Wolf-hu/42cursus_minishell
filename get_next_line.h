/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 14:43:36 by omalovic          #+#    #+#             */
/*   Updated: 2024/11/11 16:24:08 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 15
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
// # include <fcntl.h>

int	ft_strlen(char *str);
char	*get_remainder(char *buffer, int checker_result, int bytes_read);
void	free_str(char *str);
int	checker(char *buffer);

#endif
